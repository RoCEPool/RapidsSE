#!/bin/bash

# This is a generic cloud-init script that can be used when starting a new EC2 instance.

echo
echo
echo
echo "*************************************"
echo "Starting the cloud-init setup script."
echo "*************************************"

# Partition, format and mount the instance stores if they are not already mounted.
# This needs to be done whenever the instance is stopped, but not when it is rebooted.

echo
echo "*** Installing the SSD mount script ***"
echo
cat << 'EOF' >> "/etc/rc.d/rc.local"

# ----------------------------------------------------------------------------------------
# Setup the SSDs on each boot

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
    chown -R ec2-user:wheel "${mount_point}"
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
# Setting JAVA_HOME
echo
echo "*** Setting JAVA_HOME ***"
echo
cat << EOF > /etc/profile.d/java_home.sh
#!/bin/bash
export JAVA_HOME="$(readlink -f /usr/bin/java | sed "s:/bin/java::")"
EOF

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
amazon-linux-extras install epel
yum update -y
yum install -y gcc-c++ git git-lfs java-11-amazon-corretto-headless cmake libuuid-devel
yum install -y nload nethogs bmon iftop iperf3 iotop

git lfs install

# ----------------------------------------------------------------------------------------
# Set up SSH.
echo
echo "*** Installing a common internal SSH key ***"
echo

# All machines have the same public/private key
SSH_DIR="/home/ec2-user/.ssh"

mkdir -p "${SSH_DIR}" || exit

cat << EOF >> "${SSH_DIR}/authorized_keys"
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDHlTysxd228irQB6IiQj7LHFCtxLRD2jsYTcNkh9DcIqZ3gKjGIk6OmXE1LMhCBh5Y1e5DHTGeBPIgb2Clcj2N+lZI+H/T42Ysu0hUrllEjTsM35OPZu92azL1WnmZn8VWzGta8+mtDg/2PI8pUn9Z3ZxEoyd6TYm5k/CrZquH93mUT9jKVsNUsMNrwdYdgcJ9W6+K/hAqv87MPr28F7vb8/mQbAaB5d+PeOxrE4wEFl9fw7uh1m28NjXbyOLEbw6F/D3Ev5+3PZ50qKzQ9QFP2xFGHzT9Zk3jI76t1PBv/ZHrQ++XmWmy+CFfIaUVy2Dcf1JwnEH311nw5X83cGOv ec2-user@localhost
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
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDHlTysxd228irQB6IiQj7LHFCtxLRD2jsYTcNkh9DcIqZ3gKjGIk6OmXE1LMhCBh5Y1e5DHTGeBPIgb2Clcj2N+lZI+H/T42Ysu0hUrllEjTsM35OPZu92azL1WnmZn8VWzGta8+mtDg/2PI8pUn9Z3ZxEoyd6TYm5k/CrZquH93mUT9jKVsNUsMNrwdYdgcJ9W6+K/hAqv87MPr28F7vb8/mQbAaB5d+PeOxrE4wEFl9fw7uh1m28NjXbyOLEbw6F/D3Ev5+3PZ50qKzQ9QFP2xFGHzT9Zk3jI76t1PBv/ZHrQ++XmWmy+CFfIaUVy2Dcf1JwnEH311nw5X83cGOv ec2-user@$HOSTNAME
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

chown -R ec2-user:ec2-user "${SSH_DIR}"

# ----------------------------------------------------------------------------------------
# Vim annoyances
echo
echo "*** Setting vim preferences ***"
echo

vimrc="/home/ec2-user/.vimrc"
cat << EOF > "${vimrc}"
set ts=2
set sw=2
set noai
set smarttab
set expandtab
syntax on
set background=dark
set statusline+=%F
set laststatus=2
EOF

chown ec2-user:ec2-user "${vimrc}"

# ----------------------------------------------------------------------------------------
# Bash prompt
echo
echo "*** Setting bash preferences ***"
echo

bash_profile="/home/ec2-user/.bash_profile"
echo "" >> "${bash_profile}"
echo '#export PS1="[\u@\h \W]\$ ' >> "${bash_profile}"
echo 'export PS1="\[\e[0;33m\]\u\[\e[0m\]@\[\e[0;32m\]\h\[\e[0m\]:\[\e[38;5;033m\]\W\[\e[0m\]\$ "' >> "${bash_profile}"
echo "" >> "${bash_profile}"

# ----------------------------------------------------------------------------------------

echo "**************************************"
echo "Completed the cloud-init setup script!"
echo "**************************************"
echo
echo
echo