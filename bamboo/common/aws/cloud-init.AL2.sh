#!/bin/bash
# shellcheck disable=SC2164

# This is a generic cloud-init script that can be used when starting a new EC2 instance.

# We wrap everything in a subshell to redirect all output through the tee command
# and to a file of our choosing.
CLOUD_INIT_OUTPUT="/cloud-init-output.log"
TEXLIVE_OUTPUT="/cloud-init-texlive.log"
TEE_CMD=("tee" "-ia" "${CLOUD_INIT_OUTPUT}")

{
  if which tee &>/dev/null; then
    echo "Cloud init output being written out to file: ${CLOUD_INIT_OUTPUT}"
  else
    echo "ERROR: no tee utility available"
    TEE_CMD=("echo")
  fi
} > "${CLOUD_INIT_OUTPUT}"
chmod a+r "${CLOUD_INIT_OUTPUT}"

touch "${TEXLIVE_OUTPUT}"
chmod a+r "${TEXLIVE_OUTPUT}"

{
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
    fi

  elif [ -f /etc/centos-release ]; then
    OS_VER=$(cat /etc/centos-release)
    if [[ "${OS_VER}" =~ 'CentOS Linux release 7' ]]; then
      OS_DIST="${CENTOS7}"
      OS_USER="${CENTOS7_USER}"
      OS_HOME_DIR="${CENTOS7_HOME_DIR}"
    fi

  else
    echo >&2 "****** Cloud init error *******"
    echo >&2 "COULD NOT DETERMINE THE OS PLATFORM. EXITING EARLY."
    echo >&2 "****** Cloud init error *******"
  fi

  # ----------------------------------------------------------------------------------------
  # Set up SSH. We do this early so that we can still login if something goes wrong later on.
  echo
  echo "*** Installing a common internal SSH key ***"
  echo

  # All machines have the same public/private key
  SSH_DIR="${OS_HOME_DIR}/.ssh"

  mkdir -p "${SSH_DIR}" || exit

  cat <<EOF >>"${SSH_DIR}/authorized_keys"
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDHlTysxd228irQB6IiQj7LHFCtxLRD2jsYTcNkh9DcIqZ3gKjGIk6OmXE1LMhCBh5Y1e5DHTGeBPIgb2Clcj2N+lZI+H/T42Ysu0hUrllEjTsM35OPZu92azL1WnmZn8VWzGta8+mtDg/2PI8pUn9Z3ZxEoyd6TYm5k/CrZquH93mUT9jKVsNUsMNrwdYdgcJ9W6+K/hAqv87MPr28F7vb8/mQbAaB5d+PeOxrE4wEFl9fw7uh1m28NjXbyOLEbw6F/D3Ev5+3PZ50qKzQ9QFP2xFGHzT9Zk3jI76t1PBv/ZHrQ++XmWmy+CFfIaUVy2Dcf1JwnEH311nw5X83cGOv ${OS_USER}@localhost
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQDbUdCIFFt3Bq4AuJ1cBq2oB70FRNwi3pGojW7viFMu9n4/3xVczg8wLk4YghsXvQaZCNZDXHZuYqBqb0Vc3Jd5A7Cjsh3ZCmEkvxnlDA57B9ttNNHrsUH51K5tvk41z4KnNvB+idRlr/lxFG57yIMzYmPoJVz7v0ypWQ3jsasXrgCJmbXUQ55hCPx5YMeCFnwIGhdxxlwqoaKK3sPaFyvZMxJlFMzjnj/AUHm4gaTZHTX+AcnV/hPJ2zygZpTxN8yUVDmK7H8xltlSqon97Ab5dkSSORBkJG2S9B9i2j6LUWvnB6rrA23UWoirgTHlzXmzdT+nyP+AgtnoGJ2PacI+D1GXgSZKWLdxTH85YhUC0nKYeOhHlj63ymD2WLHhWigE6G34/C6Byd0pV6STFrnfMT30pIiebL6QzjrpCXQCDON7Y3QHJdDD7f/oVe5mU+Qq0tPxqtNCJzim3XAhpY74AW7NGyu2Fr0/bMGCn6GciZYtIT0IKuaObRPYzGhFmNE= alka@localhost
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCihni2+QEiGqJqBRyRa/C6t3esZoAXEMoFal5l3ocIiFxxMs2V02ACju6IjPBz8r+bJ/VNDKey3/3O1NoCXaMr9R0jEUrdfhCKBsIky7p4PLYMDYze+FMxz30Op1hJZ8O5NLhJOufR8jLcLnwsw3rm4jnX/lUM9fU4Pjd989pJ/neu+qE6Vb8inSY9kq5VB7Ksq1Vl/wtkqSpjOOCeqKnbtwxQ5dOHP6nCw6NLkgrSTz7K08DEtSlwJ29nP4FAuPYZD8YQ0p2tq0uLsP/EVCBoO4YGi+bktDhfkqWF9uzrErwpWXgvMzN0emKzYpw+rNZ37FNv9Nc17K9Wy+ssvEscFU129VVk6bF4MPd8lq6y01gOv5NWt3PaylywDsfAYE2RM5W2bAUzld3nt2zn2+H7AQbnZkrw+5QkgtelwEsWHGTM+ciWg/bndno7kS6Ej28k2E/8aTRvbeb5w0W1SIV0e8BY4S8ppDyNvskTWYeGzQoYvqL9GIQz2vE9YeVmPK0= ray@localhost
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQDEUoYT5R3u9Yzl1UuOH1o4La+Mo3cjbJqrAbWsBvzjFAASRl4FdSrn1jrnpo2kNZd31Ca7N1D8iSiaarBwXZoYWyBo+q6vr4LD8JrJZ0UwHPRI++wqQEkc9c2LSDWKfqfNfbo4MK4BxjS1cYvIEiOwbZKD9REAN83OQFQEfYrElA7iwA4Fr6iEkr5kOPbH8MeXHh1U5NMTVQZ2/mYbcmxA89NmDOcbHm5LnYNzxuUr9IujIQAWi/sKQVqj2E6G7DM7fDUrNmifbCRW8+vHcD+WXQS7c7e/PV3q2ZOWZAjD85eCCV/vTi5Fk7r0Em6LvDpMyd084DafKnPiyxxpyL2d/8DAQq9Rq29bM0aeml4K41WSXC6mCnlY0SI7gYSutmoL23sm9qNSeu994udFRowUf9DSnRoBWUmMJ/UKc6qsaCJl8h8DgIkF2YGs/pmWzgW4w5WsCfyUfiox8d+cgt96TH0Tn4EPsmqaPr72liw2ziZJpv8eUZAJD5SuvANFvxc= arjun@localhost
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABgQCx0tjHnu1xFYPEveMebvgzxLrjZ9Z2rbctSvzxkW08rUbt3Mjx9Iz5CY3IDX/yI+rv8AgGTu7BpdSSgilfYiBKJm3PEysmWB4I/CV4BUEiTU1Oxm1DnfulTYD2rrJZFG1rUv9W48w7T8tvcGGdHqyUEyqB3xUezqXVc8xtFsZaoFzQlQ7soahri71Wnx1jkeAO9ILew09V3iq+qGGv+YKEEZBVZDq7tUqwvt4qQzY4yp8pHSmsBxc/1CbGW++eXebsWADL2G7NM4MeAKXsPWEWZ6cBeDcBFqp6xMqXEV6eli+eQoH1kj6+dmbPGGlFIhamN4CunlhjGeY212b4BRtenMwydFsOIU+9sLEl6WGYWI8oXs2CviQfzNs1+R71OIZ6nGVlfkWi8bj0luJLP9HCUYf3ysstAzazgTzgQf9lc+CywYOn07DhEAad/0C5tFnAWwtQ7Ul+KO8V02Aqh5etSCQl2mqdEKfct6cySXdjrfq8948Jz3cQsSYaGgxzlS8= myvan@localhost
EOF

  cat <<EOF >${SSH_DIR}/id_rsa
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

  cat <<EOF >"${SSH_DIR}/id_rsa.pub"
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDHlTysxd228irQB6IiQj7LHFCtxLRD2jsYTcNkh9DcIqZ3gKjGIk6OmXE1LMhCBh5Y1e5DHTGeBPIgb2Clcj2N+lZI+H/T42Ysu0hUrllEjTsM35OPZu92azL1WnmZn8VWzGta8+mtDg/2PI8pUn9Z3ZxEoyd6TYm5k/CrZquH93mUT9jKVsNUsMNrwdYdgcJ9W6+K/hAqv87MPr28F7vb8/mQbAaB5d+PeOxrE4wEFl9fw7uh1m28NjXbyOLEbw6F/D3Ev5+3PZ50qKzQ9QFP2xFGHzT9Zk3jI76t1PBv/ZHrQ++XmWmy+CFfIaUVy2Dcf1JwnEH311nw5X83cGOv "${OS_USER}"@$HOSTNAME
EOF

  cat <<EOF >>"${SSH_DIR}/known_hosts"
nat,192.168.0.5 ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBNQ+5ZJflMCBWm+cvzjKtJK47OsD8bi42hyp7z//PE6ucc2CWrDZDjMI/Vw8T+oRT+8HaS23YPoXrrLAFUlJTuQ=
EOF

  cat <<EOF >"${SSH_DIR}/config"
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

    cat <<EOF >>"${SSH_DIR}/authorized_keys"
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC7KEGvt+IqlHt0GcN1WwpvBFqbVJQTl6s5ES+rPumXZ+KZgvI/wslAKr4aBfoWBmUSBOGgorIuh2KPlopfiSl8wDUo9VKMMHOYR3hMiiiwMGAA6YCPfRn4zKM97UrU0ESaLYbECBpLtnc+sCDoBbz7b5ZlAbWV83CCBSku5PnaHDBTU5fAikM2TG82aqlyq0iyiGMR5tX7iIHFSfyHHHobPPvoNY6Kafq/hkPeGfQ1m8AMU/9834H3f3I7L1rFOJYmmYfG3CS45SVgYoAfRTMBRW4nLjaNSWVtTgQ+9HIFOVgGMZd1supaYKSx0ubfhpWbJW4y69PwBAaR7Qdr57nn root@ip-192-168-6-9.ec2.internal
EOF

    cat <<EOF >${SSH_DIR}/id_rsa
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

    cat <<EOF >"${SSH_DIR}/id_rsa.pub"
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC7KEGvt+IqlHt0GcN1WwpvBFqbVJQTl6s5ES+rPumXZ+KZgvI/wslAKr4aBfoWBmUSBOGgorIuh2KPlopfiSl8wDUo9VKMMHOYR3hMiiiwMGAA6YCPfRn4zKM97UrU0ESaLYbECBpLtnc+sCDoBbz7b5ZlAbWV83CCBSku5PnaHDBTU5fAikM2TG82aqlyq0iyiGMR5tX7iIHFSfyHHHobPPvoNY6Kafq/hkPeGfQ1m8AMU/9834H3f3I7L1rFOJYmmYfG3CS45SVgYoAfRTMBRW4nLjaNSWVtTgQ+9HIFOVgGMZd1supaYKSx0ubfhpWbJW4y69PwBAaR7Qdr57nn root@ip-192-168-6-9.ec2.internal
EOF

    touch "${SSH_DIR}/known_hosts"

    cat <<EOF >"${SSH_DIR}/config"
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

  # Partition, format and mount the instance stores if they are not already mounted.
  # This needs to be done whenever the instance is stopped, but not when it is rebooted.

  echo
  echo "*** Installing the SSD mount script ***"
  echo
  cat <<'EOF' >>"/etc/rc.d/rc.local"

# ----------------------------------------------------------------------------------------
# Setup the SSDs on each boot

LOCAL_USER="ec2-user"
if [[ -f /etc/system-release ]] && [[ "$(cat /etc/system-release)" =~ 'CentOS Linux' ]]; then
  LOCAL_USER="centos"
fi

if ! mount | grep "/mnt/disk1" &>/dev/null; then

  # Get the name of any device (sans "/dev/") that is mounted under /
  # including the root filesystem, as we don't want to format this device.
  # e.g. /dev/nvme0n1 --> nvme0n1
  ignore_devices=($(df -h | grep -E "^/dev/" | awk '{if($6 ~ "^/") print $1;}' | cut -c 6-))

  # Get the list of block devices in the system.
  ssds=($(lsblk -d -n -o NAME))
  counter=0

  for device in "${ssds[@]}"
  do
    # Ignore devices that are already mounted and devices that are a subset of
    # the device the root filesystem is mounted on.
    ignore_device=0
    for ignore_device_name in ${ignore_devices[@]}; do
      if [[ "${ignore_device_name}" =~ ^"${device}".* ]]; then
        ignore_device=1
        break
      fi
    done

    [ "${ignore_device}" -eq 1 ] && echo "Ignoring device ${device}" && continue

    # ignore non-nvme devices
    if ! [[ "${device}" =~ ^nvme.* ]]
    then
      # ignore this device
      echo "Ignoring device ${device}"
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

    sleep 2

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
  # Give more memory to /dev/shm. Use 95% of system memory, or mem-10GB, whichever is larger.
  SHM_SIZE=$(cat /proc/meminfo | grep "MemTotal:" | awk '{m1 = $2/1048576*0.95; m2 = $2/1048576 - 10; if (m1 > m2) {m3 = m1} else {m3 = m2}; printf "%d\n", m3}')
  if [ "${SHM_SIZE}" -eq 0 ]; then
    SHM_SIZE=1
  fi
  echo "Updating size of /dev/shm to 95% of system memory"
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
  echo "search local" >>/etc/resolv.conf

  # ----------------------------------------------------------------------------------------
  # /etc/hosts shortcuts
  echo
  echo "*** Adding 192.168.0.5 as a shortcut for ci.rapidsdata.com to /etc/hosts ***"
  echo
  echo "192.168.0.5 ci.rapidsdata.com" >>/etc/hosts
  # Otherwise it costs us money when we transfer data via the elastic IP associated with
  # this URL.

  # ----------------------------------------------------------------------------------------
  # Setting IGMP to v2 instead of v3 (for multicast)
  echo
  echo "*** Setting IGMP to v2 ***"
  echo
  sysctl net.ipv4.conf.eth0.force_igmp_version=2
  {
    echo
    echo "# Enable Multicast on AWS"
    echo "net.ipv4.conf.eth0.force_igmp_version=2"
    echo
  } >>/etc/sysctl.conf

  # ----------------------------------------------------------------------------------------
  # Change permissions to allow for writing to /opt
  echo
  echo "*** Setting /opt permissions ***"
  echo
  chown root:wheel /opt
  chmod g+w /opt


  # ----------------------------------------------------------------------------------------
  # Installing AWS CLI
  # Note that we retry any network operations as these can fail or timeout for
  # various reasons. A timeout means that we ultimately cannot connect to this
  # machine.
  echo
  echo "*** Installing AWS CLI ***"
  echo
  if ! which aws; then
    # AWS CLI needs curl and unzip
    while ! yum install -y curl wget unzip; do
      sleep 1
    done

    while ! curl -L "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"; do
      sleep 1
    done

    unzip awscliv2.zip
    ./aws/install
  fi

  # ----------------------------------------------------------------------------------------
  # Install Tex Live in parallel in the background to /usr/local/texlive.
  # This is for generating PDF documentation.
  (
    echo
    echo "*** Installing TexLive ***"
    echo
    cd /usr/local
    aws s3 cp "s3://rapidsse-build-support/texlive/texlive.2022.${OS_DIST}.tar.gz" - | tar xz
    chown -R root:root /usr/local/texlive
    chmod -R g-w /usr/local/texlive

    echo
    echo "Setting up TexLive environment variables"
    {
      echo
      echo 'TEXLIVEDIR="/usr/local/texlive"'
      echo 'PATH="${PATH}:${TEXLIVEDIR}/bin/x86_64-linux:${TEXLIVEDIR}/bin/aarch64-linux"'
      echo 'MANPATH="${MANPATH}:${TEXLIVEDIR}/texmf-dist/doc/man"'
      echo 'INFOPATH="${INFOPATH}:${TEXLIVEDIR}/texmf-dist/doc/info"'
      echo
      echo 'export PATH MANPATH INFOPATH'
      echo
    } >>/etc/profile.d/sh.local

    echo
    echo "TexLive installation complete"
  ) > "${TEXLIVE_OUTPUT}" &

  # ----------------------------------------------------------------------------------------
  # Apply any package updates
  echo
  echo "*** Installing updates and necessary packages ***"
  echo
  amazon-linux-extras install -y epel
  yum update -y
  yum install -y gcc-c++ git git-lfs java-11-amazon-corretto-headless cmake libuuid-devel unzip nmap-ncat ncurses parallel python3 perl-Digest-MD5 perl-Env zip
  yum install -y nload nethogs bmon iftop iperf3 iotop jq telnet

  if [ "${OS_DIST}" = "${CENTOS7}" ]; then
    yum install -y epel-release
    yum install -y java vim mysql jq
  fi

  # ShellCheck
  echo
  echo "*** Shellcheck ***"
  (
    cd /usr/local/bin
    while ! curl -L "https://github.com/koalaman/shellcheck/releases/download/stable/shellcheck-stable.linux.x86_64.tar.xz" -o "shellcheck-stable.linux.x86_64.tar.xz"; do
      sleep 1
    done
    tar xvJf shellcheck-stable.linux.x86_64.tar.xz --strip-components=1 shellcheck-stable/shellcheck
    chown root:root shellcheck
    rm -f shellcheck-stable.linux.x86_64.tar.xz
  )

  # ----------------------------------------------------------------------------------------
  # Install JMeter to /opt/jmeter
  echo
  echo "*** Installing jmeter ***"
  echo
  pushd "$(pwd)"
  cd /opt
  JMETER_VER="5.6"
  JMETER_NAME="apache-jmeter-${JMETER_VER}"
  JMETER_FILE="${JMETER_NAME}.tgz"
  if ! curl --fail "https://dlcdn.apache.org//jmeter/binaries/${JMETER_FILE}" -o "${JMETER_FILE}"; then
    echo "ERROR: Could not download JMeter - the version might be old and may need to be updated."
  else
    tar xvzf "${JMETER_FILE}"
    rm -f "$JMETER_FILE"
    ln -s "${JMETER_NAME}" jmeter
    chown -R ec2-user:ec2-user "${JMETER_NAME}"
    chown ec2-user:ec2-user jmeter
  fi

  popd

  # Add this JMeter installation to the PATH for the ec2-user
  bash_profile="${OS_HOME_DIR}/.bash_profile"

  {
    echo ""
    # shellcheck disable=SC2016
    echo 'export PATH=/opt/jmeter/bin:${PATH}'
    echo ""
  } >>"${bash_profile}"

  # ----------------------------------------------------------------------------------------
  # Installing Sphinx for documentation generation
  echo
  echo "*** Installing Sphinx ***"
  echo
  # Sphinx dependency, urllib3, has issues on AmazonLinux2 when it's version is >= 2.0.
  # So install an older version.
  sudo -H -u "${OS_USER}" python3 -m pip install --user -U 'urllib3<2.0'
  sudo -H -u "${OS_USER}" python3 -m pip install --user -U sphinx

  # ----------------------------------------------------------------------------------------
  # Setting JAVA_HOME
  echo
  echo "*** Setting JAVA_HOME ***"
  echo
  cat <<EOF >/etc/profile.d/java_home.sh
#!/bin/bash
export JAVA_HOME="$(readlink -f /usr/bin/java | sed "s:/bin/java::")"
EOF

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

  cat <<-'EOF' >>"${SPOT_NOTIFIER}"
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
  cat <<EOF >"${vimrc}"
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
  {
    echo "";
    echo '#export PS1="[\u@\h \W]\$ ';
    echo 'export PS1="\[\e[0;33m\]\u\[\e[0m\]@\[\e[0;32m\]\h\[\e[0m\]:\[\e[38;5;033m\]\W\[\e[0m\]\$ "';
    echo "";
  } >> "${bash_profile}"

  # ----------------------------------------------------------------------------------------
  # Git setup
  echo
  echo "*** Git LFS initialisation ***"
  echo
  sudo -H -u "${OS_USER}" git lfs install

  # ----------------------------------------------------------------------------------------
  # Remove annoying GNU Parallel citation message
  echo
  echo "*** GNU Parallel citation ***"
  echo
  if which parallel &>/dev/null; then
    echo -e "will cite\n" | parallel --citation
  fi

  # ----------------------------------------------------------------------------------------
  # Indicate cloud-init has completed by dropping a file in /tmp
  echo "$(date) Completed cloud-init script" >/tmp/cloud-init-complete

  echo "**************************************"
  echo "Completed the cloud-init setup script!"
  echo "**************************************"
  echo
  echo
  echo
} 2>&1 | "${TEE_CMD[@]}"