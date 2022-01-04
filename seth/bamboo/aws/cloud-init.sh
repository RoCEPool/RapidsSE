#!/bin/bash

# This is a generic cloud-init script that can be used when starting a new EC2 instance.

echo
echo
echo
echo "*************************************"
echo "Starting the cloud-init setup script."
echo "*************************************"

readonly CENTOS7="centos7"
readonly AL2="amazon2"

readonly AL2_USER="ec2-user"
readonly CENTOS7_USER="centos"

readonly AL2_HOME_DIR="/home/${AL2_USER}"
readonly CENTOS7_HOME_DIR="/home/${CENTOS7_USER}"

OS_DIST="unknown"
OS_USER="unknown"
OS_HOME_DIR="unknown"

if [ -f /etc/system-release ]; then
  OS_VER=$(cat /etc/system-release)

  if [[ "${OS_VER}" =~ 'Amazon Linux release 2' ]]; then
    OS_DIST="${AL2}"
    OS_USER="${AL2_USER}"
    OS_HOME_DIR="${AL2_HOME_DIR}"

  elif [[ "${OS_VER}" =~ 'CentOS Linux release 7' ]]; then
    OS_DIST="${CENTOS7}"
    OS_USER="${CENTOS7_USER}"
    OS_HOME_DIR="${CENTOS7_HOME_DIR}"
  fi;

elif [ -f /etc/centos-release ]; then
  OS_VER=$(cat /etc/centos-release)
  if [[ "${OS_VER}" =~ 'CentOS Linux release 7' ]]; then
    OS_DIST="${CENTOS7}"
    OS_USER="${CENTOS7_USER}"
    OS_HOME_DIR="${CENTOS7_HOME_DIR}"
  fi;

else
  >&2 echo "****** Cloud init error *******"
  >&2 echo "COULD NOT DETERMINE THE OS PLATFORM. EXITING EARLY."
  >&2 echo "****** Cloud init error *******"
fi


# Partition, format and mount the instance stores if they are not already mounted.
# This needs to be done whenever the instance is stopped, but not when it is rebooted.

echo
echo "*** Installing the SSD mount script ***"
echo
cat << 'EOF' >> "/etc/rc.d/rc.local"

# ----------------------------------------------------------------------------------------
# Setup the SSDs on each boot

LOCAL_USER="ec2-user"
if [[ -f /etc/system-release ]] && [[ "$(cat /etc/system-release)" =~ 'CentOS Linux' ]]; then
  LOCAL_USER="centos"
fi

if ! mount | grep "/mnt/disk1" &>/dev/null; then

  # Get the name of the device (sans "/dev/") that the root filesystem is mounted on.
  # e.g. /dev/nvme0n1 --> nvme0n1
  # We don't want to partition/format this disk!
  root_device=$(df -h | grep -E "^/dev/" | cut -d ' ' -f 1 | cut -c 6-)

  # Get the list of block devices in the system.
  ssds=($(lsblk -d -n -o NAME))
  counter=0

  for device in "${ssds[@]}"
  do
    # ignore block devices that are a subset of the device the root filesystem is mounted on.
    if [[ "${root_device}" =~ ^"${device}".* ]]
    then
      # ignore this device
      echo "Ignoring root device ${device}"
      continue
    fi

    device="/dev/${device}"

    echo "Partitioning device ${device}"

    (
      echo "o"  # Create DOS partition table
      echo "n"  # Create a new partition
      echo "p"  # New partition is a primary partition
      echo "1"  # Partition #1
      echo ""   # Start the new partition at the first sector
      echo ""   # Make the new partition as big as possible
      echo "w"  # write the changes and exit
    ) | fdisk "${device}" || exit

    sleep 1

    # Get the partition name
    partition=$(lsblk -l -n -p -oNAME "${device}" | tail -n 1)

    echo "Formatting partition ${partition}"
    counter=$((counter + 1))
    mount_point="/mnt/disk${counter}"

    mkfs.ext4 -E nodiscard -m0 "${partition}" || exit

    echo "Mounting partition ${partition} to ${mount_point}"

    mkdir -p "${mount_point}"
    mount -o discard "${partition}" "${mount_point}" || exit
    chown -R "${LOCAL_USER}":wheel "${mount_point}"
    chmod g+w "${mount_point}"

    MOUNTS+=("${mount_point}")
  done
fi
EOF

# Run the above script
echo
echo "*** Running the SSD mount script ***"
echo
chmod +x /etc/rc.d/rc.local
bash /etc/rc.d/rc.local

# ----------------------------------------------------------------------------------------
# Give more memory to /dev/shm. Use 95% of system memory
echo "Updating size of /dev/shm to 95% of system memory"
SHM_SIZE=$(cat /proc/meminfo | grep "MemTotal:" | awk '{printf "%d\n", $2/1048576*0.95}')
{
  echo ""
  echo "# Set /dev/shm to 95% of system memory"
  echo "tmpfs     /dev/shm     tmpfs     defaults,size=${SHM_SIZE:-500}g     0 0 "
} >>/etc/fstab
mount -o remount /dev/shm

# ----------------------------------------------------------------------------------------
# DNS (Route 53) resolution
# This will force unqualified names to tried with the .local domain name and
# then be looked up in DNS.
echo
echo "*** Setting DNS domain name search qualifier ***"
echo
echo "search local" >> /etc/resolv.conf

# ----------------------------------------------------------------------------------------
# /etc/hosts shortcuts
echo
echo "*** Adding 192.168.0.5 as a shortcut for ci.rapidsdata.com to /etc/hosts ***"
echo
echo "192.168.0.5 ci.rapidsdata.com" >> /etc/hosts
# Otherwise it costs us money when we transfer data via the elastic IP associated with
# this URL.

# ----------------------------------------------------------------------------------------
# Change permissions to allow for writing to /opt
echo
echo "*** Setting /opt permissions ***"
echo
chown root:wheel /opt
chmod g+w /opt

# ----------------------------------------------------------------------------------------
# Apply any package updates
echo
echo "*** Installing updates and necessary packages ***"
echo
amazon-linux-extras install -y epel
yum update -y
yum install -y gcc-c++ git git-lfs java-11-amazon-corretto-headless cmake libuuid-devel unzip
yum install -y nload nethogs bmon iftop iperf3 iotop jq telnet

if [ "${OS_DIST}" = "${CENTOS7}" ]; then
  yum install -y epel-release
  yum install -y java vim mysql python3 jq
fi

git lfs install

# ----------------------------------------------------------------------------------------
# Installing AWS CLI
if ! which aws; then
  curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
  unzip awscliv2.zip
  ./aws/install
fi

# ----------------------------------------------------------------------------------------
# Setting JAVA_HOME
echo
echo "*** Setting JAVA_HOME ***"
echo
cat << EOF > /etc/profile.d/java_home.sh
#!/bin/bash
export JAVA_HOME="$(readlink -f /usr/bin/java | sed "s:/bin/java::")"
EOF


# ----------------------------------------------------------------------------------------
# Set up SSH.
echo
echo "*** Installing a common internal SSH key ***"
echo

# All machines have the same public/private key
SSH_DIR="${OS_HOME_DIR}/.ssh"

mkdir -p "${SSH_DIR}" || exit

cat << EOF >> "${SSH_DIR}/authorized_keys"
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDHlTysxd228irQB6IiQj7LHFCtxLRD2jsYTcNkh9DcIqZ3gKjGIk6OmXE1LMhCBh5Y1e5DHTGeBPIgb2Clcj2N+lZI+H/T42Ysu0hUrllEjTsM35OPZu92azL1WnmZn8VWzGta8+mtDg/2PI8pUn9Z3ZxEoyd6TYm5k/CrZquH93mUT9jKVsNUsMNrwdYdgcJ9W6+K/hAqv87MPr28F7vb8/mQbAaB5d+PeOxrE4wEFl9fw7uh1m28NjXbyOLEbw6F/D3Ev5+3PZ50qKzQ9QFP2xFGHzT9Zk3jI76t1PBv/ZHrQ++XmWmy+CFfIaUVy2Dcf1JwnEH311nw5X83cGOv ${OS_USER}@localhost
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQDbUdCIFFt3Bq4AuJ1cBq2oB70FRNwi3pGojW7viFMu9n4/3xVczg8wLk4YghsXvQaZCNZDXHZuYqBqb0Vc3Jd5A7Cjsh3ZCmEkvxnlDA57B9ttNNHrsUH51K5tvk41z4KnNvB+idRlr/lxFG57yIMzYmPoJVz7v0ypWQ3jsasXrgCJmbXUQ55hCPx5YMeCFnwIGhdxxlwqoaKK3sPaFyvZMxJlFMzjnj/AUHm4gaTZHTX+AcnV/hPJ2zygZpTxN8yUVDmK7H8xltlSqon97Ab5dkSSORBkJG2S9B9i2j6LUWvnB6rrA23UWoirgTHlzXmzdT+nyP+AgtnoGJ2PacI+D1GXgSZKWLdxTH85YhUC0nKYeOhHlj63ymD2WLHhWigE6G34/C6Byd0pV6STFrnfMT30pIiebL6QzjrpCXQCDON7Y3QHJdDD7f/oVe5mU+Qq0tPxqtNCJzim3XAhpY74AW7NGyu2Fr0/bMGCn6GciZYtIT0IKuaObRPYzGhFmNE= alka@localhost
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCihni2+QEiGqJqBRyRa/C6t3esZoAXEMoFal5l3ocIiFxxMs2V02ACju6IjPBz8r+bJ/VNDKey3/3O1NoCXaMr9R0jEUrdfhCKBsIky7p4PLYMDYze+FMxz30Op1hJZ8O5NLhJOufR8jLcLnwsw3rm4jnX/lUM9fU4Pjd989pJ/neu+qE6Vb8inSY9kq5VB7Ksq1Vl/wtkqSpjOOCeqKnbtwxQ5dOHP6nCw6NLkgrSTz7K08DEtSlwJ29nP4FAuPYZD8YQ0p2tq0uLsP/EVCBoO4YGi+bktDhfkqWF9uzrErwpWXgvMzN0emKzYpw+rNZ37FNv9Nc17K9Wy+ssvEscFU129VVk6bF4MPd8lq6y01gOv5NWt3PaylywDsfAYE2RM5W2bAUzld3nt2zn2+H7AQbnZkrw+5QkgtelwEsWHGTM+ciWg/bndno7kS6Ej28k2E/8aTRvbeb5w0W1SIV0e8BY4S8ppDyNvskTWYeGzQoYvqL9GIQz2vE9YeVmPK0= ray@localhost
EOF

cat << EOF > ${SSH_DIR}/id_rsa
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAx5U8rMXdtvIq0AeiIkI+yxxQrcS0Q9o7GE3DZIfQ3CKmd4Co
xiJOjplxNSzIQgYeWNXuQx0xngTyIG9gpXI9jfpWSPh/0+NmLLtIVK5ZRI07DN+T
j2bvdmsy9Vp5mZ/FVsxrWvPprQ4P9jyPKVJ/Wd2cRKMnek2JuZPwq2arh/d5lE/Y
ylbDVLDDa8HWHYHCfVuviv4QKr/OzD69vBe72/P5kGwGgeXfj3jsaxOMBBZfX8O7
odZtvDY128jixG8Ohfw9xL+ftz2edKis0PUBT9sRRh80/WZN4yO+rdTwb/2R60Pv
l5lpsvghXyGlFctg3H9ScJxB99dZ8OV/N3BjrwIDAQABAoIBADtmRi2hohAd2PIb
I30QvEGVBsoA3inHnIjqM/oKlfW+dk7gwtpdQYYdUt0hm+RSj9ueqA/fc3H1RN60
Z042Ru2nkOCc3OaCKlmrz6YsUbMeWXHGZNNsLl7PHtZ4+/Hl5ESmCFmEeIqhwcxm
T7FbtT1YwHaJHGnBA/lwd1N79hc74FK//KdIptUH9wp/0LwH7l4o5DBTtbaLOs6E
Sw4uJKh9Wy+2kSvI9673xV3fcP1M9sCfYyKqCz75rVLsogfF9dhxu85Mwvn4UTsc
gjPMc6jg5DW4FPch1pL6rxIVpS8RCdQE0FliihJz8VJdOM4NWLQHxatnkNXtN0n0
XfuSgyECgYEA8xxaXiv5JBeXJ1/Jr9AzQTS9uv3R2XTetf3nISKceBn3WL2H86ye
mG8LhQLcyHXzHJM6WRaDdDZIymWq8s00s3z7Pv+JCbJOLNuY2vuez/tHTs+DQHih
7qb1C0imxuIbPqOa/3MxSFQbGXMa8iF+jBK7OguCTejTAO7xysvmRh8CgYEA0ioZ
KP3fBK9J7yxjuxmtjMRGnjag/CGHrDpA295AyFLOXg6L6mCzqBgFgNkC5XXHVXfJ
qROSGzo8y3zqioS9dvJG9d8McEM8LppkDP7/3VcXBR3KIeolWvVzA9BtV361d0sx
+DzVY35c4gonVpq1PhVzYafMeWnn4+R3CGFmkHECgYEAknOqHMDwvUVY0Wdq4HbT
6ZwUD+mH2TAGWiwyaEupZtB9qP/EPo7mm32Juv8WccxcQAKRS8m8Nu3cu9CzQsFL
1jScRj86FpVLcsp3MrQR38jIFDn+H4CMTHBXfgyuK3FaaEm2vdxu+9tu1833lZWH
3moKT9VPEhAHzLAlCL38YpECgYEAy+hwlOcMVN1F3Lqy4LufOS7OXX8fXsGhgybZ
+vl+3V4KQBPZCMReSz9TR5pGRx+pEXGxxUvpvGUay0wdu9vif9+9nZyV0u5JG+ZM
IJfg5H41JY+rqJAnQkNXHmJBVFzp8B6TEwji+S+HKwEy/cIJC32Af0VxDHQGu4PV
QdpOyuECgYBooJ6uV96e1faodroUdNIYQol+HX+Id7OqqRgnVkGshF9rPtXCOQG6
Z7nA5mU5vwQhGSCzm1gFdGPPNAb4alDZxNl5Y/EPeYbfzMd+8zeYQfrf4DGmOwSq
zeAErhWRpUkqMr7HoAL+ZaEuzdNZbr6bz+EibfRJxz67Tf68AT6wyQ==
-----END RSA PRIVATE KEY-----
EOF

cat << EOF > "${SSH_DIR}/id_rsa.pub"
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDHlTysxd228irQB6IiQj7LHFCtxLRD2jsYTcNkh9DcIqZ3gKjGIk6OmXE1LMhCBh5Y1e5DHTGeBPIgb2Clcj2N+lZI+H/T42Ysu0hUrllEjTsM35OPZu92azL1WnmZn8VWzGta8+mtDg/2PI8pUn9Z3ZxEoyd6TYm5k/CrZquH93mUT9jKVsNUsMNrwdYdgcJ9W6+K/hAqv87MPr28F7vb8/mQbAaB5d+PeOxrE4wEFl9fw7uh1m28NjXbyOLEbw6F/D3Ev5+3PZ50qKzQ9QFP2xFGHzT9Zk3jI76t1PBv/ZHrQ++XmWmy+CFfIaUVy2Dcf1JwnEH311nw5X83cGOv "${OS_USER}"@$HOSTNAME
EOF

cat << EOF >> "${SSH_DIR}/known_hosts"
nat,192.168.0.5 ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBNQ+5ZJflMCBWm+cvzjKtJK47OsD8bi42hyp7z//PE6ucc2CWrDZDjMI/Vw8T+oRT+8HaS23YPoXrrLAFUlJTuQ=
EOF

cat << EOF > "${SSH_DIR}/config"
Host *
  StrictHostKeyChecking no
EOF

chmod 700 "${SSH_DIR}"
chmod 600 "${SSH_DIR}/authorized_keys"
chmod 600 "${SSH_DIR}/id_rsa"
chmod 600 "${SSH_DIR}/config"
chmod 644 "${SSH_DIR}/id_rsa.pub"
chmod 644 "${SSH_DIR}/known_hosts"

chown -R "${OS_USER}:${OS_USER}" "${SSH_DIR}"


# ----------------------------------------------------------------------------------------
# Setup root SSH keys for CentOS (needed by Memsql)

if [ "${OS_DIST}" = "${CENTOS7}" ]; then


echo
echo "*** Installing a root SSH key for Memsql ***"
echo

# All machines have the same public/private key
SSH_DIR="/root/.ssh"

mkdir -p "${SSH_DIR}" || exit

cat << EOF >> "${SSH_DIR}/authorized_keys"
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC7KEGvt+IqlHt0GcN1WwpvBFqbVJQTl6s5ES+rPumXZ+KZgvI/wslAKr4aBfoWBmUSBOGgorIuh2KPlopfiSl8wDUo9VKMMHOYR3hMiiiwMGAA6YCPfRn4zKM97UrU0ESaLYbECBpLtnc+sCDoBbz7b5ZlAbWV83CCBSku5PnaHDBTU5fAikM2TG82aqlyq0iyiGMR5tX7iIHFSfyHHHobPPvoNY6Kafq/hkPeGfQ1m8AMU/9834H3f3I7L1rFOJYmmYfG3CS45SVgYoAfRTMBRW4nLjaNSWVtTgQ+9HIFOVgGMZd1supaYKSx0ubfhpWbJW4y69PwBAaR7Qdr57nn root@ip-192-168-6-9.ec2.internal
EOF

cat << EOF > ${SSH_DIR}/id_rsa
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAuyhBr7fiKpR7dBnDdVsKbwRam1SUE5erOREvqz7pl2fimYLy
P8LJQCq+GgX6FgZlEgThoKKyLodij5aKX4kpfMA1KPVSjDBzmEd4TIoosDBgAOmA
j30Z+MyjPe1K1NBEmi2GxAgaS7Z3PrAg6AW8+2+WZQG1lfNwggUpLuT52hwwU1OX
wIpDNkxvNmqpcqtIsohjEebV+4iBxUn8hxx6Gzz76DWOimn6v4ZD3hn0NZvADFP/
fN+B939yOy9axTiWJpmHxtwkuOUlYGKAH0UzAUVuJy42jUllbU4EPvRyBTlYBjGX
dbLqWmCksdLm34aVmyVuMuvT8AQGke0Ha+e55wIDAQABAoIBAQCby6AagW0Q2veF
pyixGW8vCSXPpfF5+ch7wdW3rLBQ7BaSiZHVB/X1p18GVLFhAkPCYzPbP3QqDWM7
eiqH6qsRgQL/rG/h6TdgrSX0ORIz0KEqQTZWu+IR3W+KTmz7iL+CxKEpsEELg8Hx
+jX6T0SI3dQJEwow4Cxo35jvw4ZFS8F2Gq+5wSX2ZY+FGI1u+Gxkrq71IjuO3+Q/
unTtjj/EwnW6fdJaoBZWBiTqEpMJ8XFEJKQjfwRRhYNeFLiCGB4UPBVRH9MZNEGo
s0p3/DareqxJGVfW9os//QGNoEaeevZ6mbq/dTaz2FwkU4RQs71R8Ut7Ueg9X7yI
mJ0hLrDBAoGBAPMMmW0bEs4fOhfiIwEqiRe6qFvvL4sdCRLDYebuUJ5phlku36Wn
9/Z5yWy3tvTqhuyTHlqHyhz5pvWO0d58Ic1EVHyCyweNABVrvu+BT7ExgB8kgODF
s1cLC1Rnt1ia4OQncLUwJTeqdqPl6PJOiNHHqRVMReYf4cc78kgVk2ZhAoGBAMUh
PiO8pfKKuIlF8toOt3aMX72wc9H9UOu9q2ECmi1SBcvInwd2q54RqGxw8sx691eP
bKFBGzIADn5ft58GaQBbamAC/YzBK9qtUoZBMbkXQT5hmomDXVdrl7s+zGvpMgo3
MAPXTsx+5oapS0xp8wYP+ozmh24Acq/XizLDvHVHAoGBAL78+Jd2/dkBSto3tkAV
V7NQ1zmfA+IyR835wLYCMVG6JHtyTQZUrpJ9cx2+Y8Ojg7DljqWfLOCGAX/W4e4N
51Xwyt8ovDhLwXQTve9g0umvtzlBqxB16o6MvDNwaO/0jKJcp8GJesV6a04xvVKj
CfqnBaQZ0peaGAZCggnJApIhAoGAHs6dKXSX8pmfUUSvOUjHfy1c4QYsc4rFFLJl
kBmaAq8o3LoTsFTpaSCVU60rJK2q7fIqJeFI4L2FhlZrGd0kl7uxmiOdgFvIALrJ
hNW3cZeiyJ70Sb1P3xOfeNHNnKeq/FG3nUIOvIKwgCcA74Y3RqFEpLR6QcSsJ7Ll
vB8+2o0CgYAtIof58hlq56TFXsvYNvM3SK0Sso24Dok0CHv6fTdfPTmsKecYw/lM
qp5saDBWa7O+p6M3ctIrQ2usHO3e9HNA9Y3T+fQq8g1i8Nx7G5cJH+mKNFZzcFj3
AUC4Zj0QsEAnWU+AtQKhiX98xsvYONFWP5bHl7cStpgNKQDeH33JSA==
-----END RSA PRIVATE KEY-----
EOF

cat << EOF > "${SSH_DIR}/id_rsa.pub"
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC7KEGvt+IqlHt0GcN1WwpvBFqbVJQTl6s5ES+rPumXZ+KZgvI/wslAKr4aBfoWBmUSBOGgorIuh2KPlopfiSl8wDUo9VKMMHOYR3hMiiiwMGAA6YCPfRn4zKM97UrU0ESaLYbECBpLtnc+sCDoBbz7b5ZlAbWV83CCBSku5PnaHDBTU5fAikM2TG82aqlyq0iyiGMR5tX7iIHFSfyHHHobPPvoNY6Kafq/hkPeGfQ1m8AMU/9834H3f3I7L1rFOJYmmYfG3CS45SVgYoAfRTMBRW4nLjaNSWVtTgQ+9HIFOVgGMZd1supaYKSx0ubfhpWbJW4y69PwBAaR7Qdr57nn root@ip-192-168-6-9.ec2.internal
EOF

touch "${SSH_DIR}/known_hosts"

cat << EOF > "${SSH_DIR}/config"
Host *
  StrictHostKeyChecking no
EOF

chmod 700 "${SSH_DIR}"
chmod 600 "${SSH_DIR}/authorized_keys"
chmod 600 "${SSH_DIR}/id_rsa"
chmod 600 "${SSH_DIR}/config"
chmod 644 "${SSH_DIR}/id_rsa.pub"
chmod 644 "${SSH_DIR}/known_hosts"

# We need the root private key copied to an accessible place and made readable
# so that we can deploy memsql-ops on multiple machines.
cp /root/.ssh/id_rsa /mnt/disk1
chown centos:centos /mnt/disk1/id_rsa

fi


# ----------------------------------------------------------------------------------------
# Spot instance interruption notification
echo
echo "*** Spawning spot instance interruption notification ***"
echo

# References:
#  https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/spot-interruptions.html#spot-instance-termination-notices
#  https://aws.amazon.com/blogs/compute/best-practices-for-handling-ec2-spot-instance-interruptions/
#  https://stackoverflow.com/questions/25282661/how-do-i-broadcast-messages-to-all-bash-terminal-in-python-using-wall-command-wi

SPOT_NOTIFIER="/tmp/spotInterruptionNotifier.sh"
SPOT_NOTIFIER_OUTPUT="/tmp/spotInterruptionNotifier.nohup"

cat <<-'EOF' >> "${SPOT_NOTIFIER}"
#!/bin/bash

# This script attempts to print a message on every console when the
# current spot instance is interrupted for termination.

# Get the security token
TOKEN=$(curl -s -X PUT "http://169.254.169.254/latest/api/token" -H "X-aws-ec2-metadata-token-ttl-seconds: 21600")

IS_INTERRUPTED=0


while sleep 5; do

  # Check if the termination notification is in the metadata
  HTTP_CODE=$(curl -H "X-aws-ec2-metadata-token: ${TOKEN}" -s -w %{http_code} -o /dev/null http://169.254.169.254/latest/meta-data/spot/instance-action)

  if [[ "${HTTP_CODE}" -eq 401 ]] ; then
    echo "Refreshing Authentication Token"
    TOKEN=$(curl -s -X PUT "http://169.254.169.254/latest/api/token" -H "X-aws-ec2-metadata-token-ttl-seconds: 21600")

  elif [[ "${HTTP_CODE}" -eq 200 ]]; then
    echo "[$(date)] Host $(hostname) has received a spot interruption notice."
    IS_INTERRUPTED=1
    break

  else
    echo "[$(date)] Not Interrupted"
  fi

done

if [ "${IS_INTERRUPTED}" -ne 1 ]; then
  echo "Exiting"
  exit 0
fi

# Get the interruption time in second from now
TIME_REMAINING=$(curl -s -H "X-aws-ec2-metadata-token: ${TOKEN}" http://169.254.169.254/latest/meta-data/spot/instance-action | jq '.time | fromdate - now' | awk '{printf "%d\n", $1}' 2>/dev/null)

echo "[$(date)] Host $(hostname) has received a spot interruption notice and will shutdown in ${TIME_REMAINING} seconds." | wall

while true; do

  TIME_REMAINING=$((TIME_REMAINING-1))

  if [[ "${TIME_REMAINING}" -eq 60 || "${TIME_REMAINING}" -eq 30 || "${TIME_REMAINING}" -eq 10 ]]; then
    echo "[$(date)] Host $(hostname) has received a spot interruption notice and will shutdown in ${TIME_REMAINING} seconds." | wall
  fi

  sleep 1
done;
EOF

chmod +x "${SPOT_NOTIFIER}"
chown "${OS_USER}:${OS_USER}" "${SPOT_NOTIFIER}"

touch "${SPOT_NOTIFIER_OUTPUT}"
chown "${OS_USER}:${OS_USER}" "${SPOT_NOTIFIER_OUTPUT}"

sudo -H -u ${OS_USER} nohup "${SPOT_NOTIFIER}" &>"${SPOT_NOTIFIER_OUTPUT}" &

# ----------------------------------------------------------------------------------------
# Vim annoyances
echo
echo "*** Setting vim preferences ***"
echo

vimrc="${OS_HOME_DIR}/.vimrc"
cat << EOF > "${vimrc}"
set ts=2
set sw=2
set noai
set smarttab
set expandtab
syntax on
set background=dark
set statusline+=%F
set statusline+=\ \ col:\ %c,
set statusline+=\ line:\ %l/%L
set laststatus=2
EOF

chown "${OS_USER}:${OS_USER}" "${vimrc}"

# ----------------------------------------------------------------------------------------
# Bash prompt
echo
echo "*** Setting bash preferences ***"
echo

bash_profile="${OS_HOME_DIR}/.bash_profile"
echo "" >> "${bash_profile}"
echo '#export PS1="[\u@\h \W]\$ ' >> "${bash_profile}"
echo 'export PS1="\[\e[0;33m\]\u\[\e[0m\]@\[\e[0;32m\]\h\[\e[0m\]:\[\e[38;5;033m\]\W\[\e[0m\]\$ "' >> "${bash_profile}"
echo "" >> "${bash_profile}"

# ----------------------------------------------------------------------------------------
# Git setup
sudo -H -u "${OS_USER}" git lfs install

# ----------------------------------------------------------------------------------------
# Indicate cloud-init has completed by dropping a file in /tmp
echo "$(date) Completed cloud-init script" > /tmp/cloud-init-complete

echo "**************************************"
echo "Completed the cloud-init setup script!"
echo "**************************************"
echo
echo
echo