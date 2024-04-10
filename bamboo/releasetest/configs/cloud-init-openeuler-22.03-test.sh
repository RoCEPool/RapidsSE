#!/bin/bash
# shellcheck disable=SC2164

# This is a cloud-init script used for configuring an OpenEuler 22.03 machine for running (not building) RapidsSE.
# The base64-encoded version of this file must be less than 16kB.
# OpenEuler is based on CentOS, despite the version number being similar to Ubuntu.

CLOUD_INIT_OUTPUT="/cloud-init-output.log"
CLOUD_INIT_STATUS="/cloud-init-status.log"
CLOUD_INIT_COMPLETE="/cloud-init-complete.log"
CLOUD_INIT_COMPLETE_DEPRECATED="/tmp/cloud-init-complete.log"

TEE_CMD=("tee" "-ia" "${CLOUD_INIT_OUTPUT}")
RC_LOCAL="/etc/rc.d/rc.local"

# Functions

function status() {
  echo "$@" > "${CLOUD_INIT_STATUS}"

  echo
  echo "$@"
  echo
}

function error() {
  echo "$@" > "${CLOUD_INIT_STATUS}"

  echo >&2 "****** Cloud init error *******"
  echo >&2 "$@"
  echo >&2 "****** Cloud init error *******"
}

function retryCommand() {
local maxRetries="$1"
shift
local cmd=("$@")
local retries=1

# check that maxRetries has been provided
local re='^[0-9]+$'
if ! [[ "${maxRetries}" =~ $re ]] ; then
   error "ERROR: First parameter to function retryCommand() is not a number"
   exit 1
fi

while [ "${retries}" -le "${maxRetries}" ]; do
  if [ "${retries}" -gt 1 ]; then
    status "...retry attempt #${retries} of command: ${cmd[*]}"
  fi

  ((retries++))

  if "${cmd[@]}"; then
    true
    return
  fi

  sleep 1
done

false
return
}

##

{
  if which tee &>/dev/null; then
    echo "Cloud init output being written out to file: ${CLOUD_INIT_OUTPUT}"
  else
    echo "ERROR: no tee utility available"
    TEE_CMD=("echo")
  fi
} > "${CLOUD_INIT_OUTPUT}"
chmod a+r "${CLOUD_INIT_OUTPUT}"

{
status "Starting the cloud-init setup script for OpenEuler 22.03."

readonly OPENEULER22="openEuler22.03"
readonly OPENEULER22_USER="ec2-user"
readonly OPENEULER22_HOME_DIR="/home/${OPENEULER22_USER}"

OS_DIST="unknown"
OS_USER="unknown"
OS_HOME_DIR="unknown"

# Ensure that we are on the right OS
if [ -f /etc/system-release ]; then
  OS_VER=$(cat /etc/system-release)

  if [[ "${OS_VER}" =~ openEuler\ release\ 22.03 ]]; then
    OS_DIST="${OPENEULER22}"
    OS_USER="${OPENEULER22_USER}"
    OS_HOME_DIR="${OPENEULER22_HOME_DIR}"
  fi
fi

if [ -z "${OS_DIST}" ]; then
  error "COULD NOT DETERMINE THE OS PLATFORM. EXITING EARLY. OS_VER = ${OS_VER}"
  exit 1
fi

echo "OS_DIST = ${OS_DIST}"
echo "OS_USER = ${OS_USER}"
echo "OS_HOME_DIR = ${OS_HOME_DIR}"


# Create an ec2-user
status "Creating an new user: ec2-user"
useradd "${OS_USER}"
usermod -a -G wheel "${OS_USER}"

# Setup passwordless sudo for user ec2-user
SUDO_FILE="/etc/sudoers.d/ec2-user"
{
  echo "# Let ec2-user use sudo without a password"
  echo "ec2-user  ALL=(ALL) NOPASSWD: ALL"
} >"${SUDO_FILE}"

chmod 0400 "${SUDO_FILE}"


# Set up SSH
status "Installing a common internal SSH key"

# All machines have the same public/private key
SSH_DIR="${OS_HOME_DIR}/.ssh"

mkdir -p "${SSH_DIR}" || exit

cat <<EOF >>"${SSH_DIR}/authorized_keys"
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDHlTysxd228irQB6IiQj7LHFCtxLRD2jsYTcNkh9DcIqZ3gKjGIk6OmXE1LMhCBh5Y1e5DHTGeBPIgb2Clcj2N+lZI+H/T42Ysu0hUrllEjTsM35OPZu92azL1WnmZn8VWzGta8+mtDg/2PI8pUn9Z3ZxEoyd6TYm5k/CrZquH93mUT9jKVsNUsMNrwdYdgcJ9W6+K/hAqv87MPr28F7vb8/mQbAaB5d+PeOxrE4wEFl9fw7uh1m28NjXbyOLEbw6F/D3Ev5+3PZ50qKzQ9QFP2xFGHzT9Zk3jI76t1PBv/ZHrQ++XmWmy+CFfIaUVy2Dcf1JwnEH311nw5X83cGOv ${OS_USER}@localhost
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCW2/5HwKvFlTKcc/v+l9aE94My4lgLNiQwvPCOrlsdvFNG1eietIefii6juPyQM4tzGbMjD5gqPqU9XPRU9OzZXLsPO/EPUE9ZTCGhQcjIH1/dVBO+01hMsjn9DSW4BhCMKU9UFxw41xgai4ZfJNnJg1qYY8MLKx+dzqPHFuivkRabDPrPQoH3eDQq9VeKZyUSHTcB07Jq0sk0cTa9afhUG/d3NtCwSDgW/9NkccUquMIqTi7396Rkcu2WktLlrDo8mkripSR2yp3wTs5QZ7Lonci/Ftp47GjBWKRdZRVBilYdnibFYnbGqMR800/cLI2lXaWIkkJ0piW5N7EnI/0P aws_rapidsse
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
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDHlTysxd228irQB6IiQj7LHFCtxLRD2jsYTcNkh9DcIqZ3gKjGIk6OmXE1LMhCBh5Y1e5DHTGeBPIgb2Clcj2N+lZI+H/T42Ysu0hUrllEjTsM35OPZu92azL1WnmZn8VWzGta8+mtDg/2PI8pUn9Z3ZxEoyd6TYm5k/CrZquH93mUT9jKVsNUsMNrwdYdgcJ9W6+K/hAqv87MPr28F7vb8/mQbAaB5d+PeOxrE4wEFl9fw7uh1m28NjXbyOLEbw6F/D3Ev5+3PZ50qKzQ9QFP2xFGHzT9Zk3jI76t1PBv/ZHrQ++XmWmy+CFfIaUVy2Dcf1JwnEH311nw5X83cGOv ${OS_USER}@${HOSTNAME}
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

# OpenEuler 22.03 has a broken SSH config. They have a file
# /etc/ssh/ssh_config.d/06-hanweie.conf that contains sshd config
# options, but this file is included from /etc/ssh/ssh_config
# erroneously. This causes errors when trying to run ssh from
# OpenEuler.
# Solution is to copy the contents of this 06-hanweie.conf file
# into /etc/ssh/sshd_config and then rename the offending file.
cat /etc/ssh/ssh_config.d/06-hanweie.conf >> /etc/ssh/sshd_config
mv /etc/ssh/ssh_config.d/06-hanweie.conf /etc/ssh/ssh_config.d/06-hanweie.conf.orig

status "Setting up the rc.local file"
if [ ! -f "${RC_LOCAL}" ]; then
  {
    echo "#!/bin/sh -e"
    echo
    echo
  } > "${RC_LOCAL}"
fi
chmod +x "${RC_LOCAL}"

# Partition, format and mount the instance stores if they are not already mounted.
status "Installing the SSD mount script"
cat <<'EOF' >>"${RC_LOCAL}"

# Setup the SSDs on each boot

LOCAL_USER="ec2-user"

if ! mount | grep "/mnt/disk1" &>/dev/null; then

ignore_devices=($(df -h | grep -E "^/dev/" | awk '{if($6 ~ "^/") print $1;}' | cut -c 6-))

ssds=($(lsblk -d -n -o NAME))
counter=0

for device in "${ssds[@]}"
do
  ignore_device=0
  for ignore_device_name in ${ignore_devices[@]}; do
    if [[ "${ignore_device_name}" =~ ^"${device}".* ]]; then
      ignore_device=1
      break
    fi
  done

  [ "${ignore_device}" -eq 1 ] && echo "Ignoring device ${device}" && continue

  if ! [[ "${device}" =~ ^nvme.* ]]
  then
    continue
  fi

  device="/dev/${device}"

  echo "Partitioning device ${device}"

  (
    echo "o"
    echo "n"
    echo "p"
    echo "1"
    echo ""
    echo ""
    echo "w"
  ) | fdisk "${device}" || exit

  sleep 2

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
status "Running the SSD mount script"
bash "${RC_LOCAL}"


# Give more memory to /dev/shm
SHM_SIZE=$(cat /proc/meminfo | grep "MemTotal:" | awk '{m1 = $2/1048576*0.95; m2 = $2/1048576 - 10; if (m1 > m2) {m3 = m1} else {m3 = m2}; printf "%d\n", m3}')
if [ "${SHM_SIZE}" -eq 0 ]; then
  SHM_SIZE=1
fi
status "Updating size of /dev/shm to 95% of system memory (${SHM_SIZE} GB)"
{
  echo ""
  echo "# Set /dev/shm to 95% of system memory"
  echo "tmpfs     /dev/shm     tmpfs     defaults,size=${SHM_SIZE:-500}g     0 0 "
} >>/etc/fstab
mount -o remount /dev/shm


# DNS (Route 53) resolution
status "Setting DNS domain name search qualifier"
echo "search local" >>/etc/resolv.conf


# /etc/hosts shortcuts
status "Adding 192.168.0.5 as a shortcut for ci.rapidsdata.com to /etc/hosts"
echo "192.168.0.5 ci.rapidsdata.com" >>/etc/hosts

# Setting IGMP to v2
status "Setting IGMP to v2"
{
  echo
  echo "# Enable Multicast on AWS"
} >>/etc/sysctl.conf

# Get a list of all network devices
mapfile -t netDevices < <(ip -o -4 addr | awk '{print $2}')

for device in "${netDevices[@]}"; do
  sysctl net.ipv4.conf."${device}".force_igmp_version=2
  echo "net.ipv4.conf.${device}.force_igmp_version=2" >>/etc/sysctl.conf
done

echo "" >>/etc/sysctl.conf


# /opt permissions
status "Setting /opt permissions"
chown root:wheel /opt
chmod g+w /opt


# Apply any package updates
status "Installing system updates"
yum update -y

# epel not supported on OpenEuler
# NOTE: removed git, git-lfs

# Need to have
status "Installing gcc et al"
yum install -y gcc-c++ libatomic nmap-ncat iproute bc ncurses tar vim unzip rsync

# Nice to have
# NOTE: no bmon, nload
status "Installing useful utilities"
yum install -y java-11-openjdk-headless iperf3 iotop telnet nethogs iftop jq

status "Installing GNU parallel"
wget https://linuxsoft.cern.ch/cern/centos/7/cern/x86_64/Packages/parallel-20150522-1.el7.cern.noarch.rpm
yum install -y parallel-20150522-1.el7.cern.noarch.rpm


# Vim annoyances
status "Setting vim preferences"
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


# Bash prompt
status "Setting bash preferences"
bash_profile="${OS_HOME_DIR}/.bash_profile"
{
  echo ""
  echo 'export PS1="\[\e[0;33m\]\u\[\e[0m\]@\[\e[0;32m\]\h\[\e[0m\]:\[\e[38;5;033m\]\W\[\e[0m\]\$ "'
  echo ""
} >>"${bash_profile}"

chown "${OS_USER}:${OS_USER}" "${bash_profile}"


# Install a script that will indicate that the cloud-init is complete after rebooting
cat <<EOF >>"${RC_LOCAL}"

echo "Completed the cloud-init setup script!" > "${CLOUD_INIT_STATUS}"
echo "\$(date) Completed the cloud-init setup script!" > "${CLOUD_INIT_COMPLETE}"
echo "\$(date) Completed the cloud-init setup script!" > "${CLOUD_INIT_COMPLETE_DEPRECATED}"
echo "\$(date) Rebooted" >> "${CLOUD_INIT_OUTPUT}"
EOF

{
  echo
  echo "Completed the cloud-init setup script!"
  echo "System needs to be rebooted to apply changes."
  echo
  echo
} >> "${CLOUD_INIT_OUTPUT}"

# Reboot
status "*** Rebooting now ***"
reboot
} 2>&1 | "${TEE_CMD[@]}"