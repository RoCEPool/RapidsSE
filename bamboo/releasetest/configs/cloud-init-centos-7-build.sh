#!/bin/bash
# shellcheck disable=SC2164

# Cloud-init script for configuring CentOS 7 for building (not running) RapidsSE.
# The base64-encoded version of this file must be less than 16kB.

CI="cloud-init"
CI_OUTPUT="/${CI}-output.log"
CI_STATUS="/${CI}-status.log"
CI_COMPLETE="/${CI}-complete.log"
CI_COMPLETE_DEP="/tmp/${CI_COMPLETE}"

TEXLIVE_OUT="/${CI}-texlive.log"
TEE_CMD=("tee" "-ia" "${CI_OUTPUT}")
MAX_NET_RETRIES=10
MAX_YUM_RETRIES=3
RC_LOCAL="/etc/rc.d/rc.local"

function status() {
echo "$@" > "${CI_STATUS}"
echo -e "\n$*\n"
}

function error() {
echo "$@" > "${CI_STATUS}"
local hdr="CLOUD INIT ERROR"
echo >&2 -e "${hdr}\nERROR: $*\n${hdr}"
}

function retryCommand() {
local maxRetries="$1"
shift
local cmd=("$@")
local retries=1

local re='^[0-9]+$'
if ! [[ "${maxRetries}" =~ $re ]] ; then
   error "First parameter to function retryCommand() is not a number" && exit 1
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

{
if which tee &>/dev/null; then
  echo "Cloud init output being written out to file: ${CI_OUTPUT}"
else
  error "no tee utility available"
  TEE_CMD=("echo")
fi
} > "${CI_OUTPUT}"
chmod a+r "${CI_OUTPUT}"

touch "${TEXLIVE_OUT}"
chmod a+r "${TEXLIVE_OUT}"

{
status "Starting cloud-init for CentOS 7."

if [ -f /etc/system-release ]; then
  OS_VER=$(cat /etc/system-release)

  if [[ "${OS_VER}" =~ 'CentOS Linux release 7' ]]; then
    OS_DIST="centos7"
    OS_USER="ec2-user"
    OS_HOME="/home/ec2-user"
  fi
fi

if [ -z "${OS_DIST}" ]; then
  error "COULD NOT DETERMINE THE OS PLATFORM. EXITING EARLY. OS_VER = ${OS_VER}" && exit 1
fi

status "Creating user: ${OS_USER}"
useradd "${OS_USER}" || exit 1
usermod -a -G wheel "${OS_USER}"

SUDO_FILE="/etc/sudoers.d/${OS_USER}"
{
  echo "# Let ${OS_USER} use sudo without a password"
  echo "${OS_USER}  ALL=(ALL) NOPASSWD: ALL"
} >"${SUDO_FILE}"

chmod 0400 "${SUDO_FILE}"

status "Installing a SSH key"
SSH_DIR="${OS_HOME}/.ssh"
mkdir -p "${SSH_DIR}" || exit 1

cat <<EOF >>"${SSH_DIR}/authorized_keys"
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDHlTysxd228irQB6IiQj7LHFCtxLRD2jsYTcNkh9DcIqZ3gKjGIk6OmXE1LMhCBh5Y1e5DHTGeBPIgb2Clcj2N+lZI+H/T42Ysu0hUrllEjTsM35OPZu92azL1WnmZn8VWzGta8+mtDg/2PI8pUn9Z3ZxEoyd6TYm5k/CrZquH93mUT9jKVsNUsMNrwdYdgcJ9W6+K/hAqv87MPr28F7vb8/mQbAaB5d+PeOxrE4wEFl9fw7uh1m28NjXbyOLEbw6F/D3Ev5+3PZ50qKzQ9QFP2xFGHzT9Zk3jI76t1PBv/ZHrQ++XmWmy+CFfIaUVy2Dcf1JwnEH311nw5X83cGOv ${OS_USER}@localhost
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCW2/5HwKvFlTKcc/v+l9aE94My4lgLNiQwvPCOrlsdvFNG1eietIefii6juPyQM4tzGbMjD5gqPqU9XPRU9OzZXLsPO/EPUE9ZTCGhQcjIH1/dVBO+01hMsjn9DSW4BhCMKU9UFxw41xgai4ZfJNnJg1qYY8MLKx+dzqPHFuivkRabDPrPQoH3eDQq9VeKZyUSHTcB07Jq0sk0cTa9afhUG/d3NtCwSDgW/9NkccUquMIqTi7396Rkcu2WktLlrDo8mkripSR2yp3wTs5QZ7Lonci/Ftp47GjBWKRdZRVBilYdnibFYnbGqMR800/cLI2lXaWIkkJ0piW5N7EnI/0P aws_rapidsse
EOF

cat <<EOF >"${SSH_DIR}"/id_rsa
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
chmod 600 "${SSH_DIR}/authorized_keys" "${SSH_DIR}/id_rsa" "${SSH_DIR}/config"
chmod 644 "${SSH_DIR}/id_rsa.pub" "${SSH_DIR}/known_hosts"

chown -R "${OS_USER}:${OS_USER}" "${SSH_DIR}"

status "Setting up rc.local file"
if [ ! -f "${RC_LOCAL}" ]; then
  echo -e "#!/bin/sh -e\n\n" > "${RC_LOCAL}"
fi
chmod +x "${RC_LOCAL}"

status "Installing SSD mount script"
cat <<'EOF' >>"${RC_LOCAL}"

# Setup the SSDs
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

  [ "${ignore_device}" -eq 1 ] && continue

  if ! [[ "${device}" =~ ^nvme.* ]]
  then
    continue
  fi

  device="/dev/${device}"

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

  counter=$((counter + 1))
  mount_point="/mnt/disk${counter}"

  mkfs.ext4 -E nodiscard -m0 "${partition}" || exit

  mkdir -p "${mount_point}"
  mount -o discard "${partition}" "${mount_point}" || exit
  chown -R "${LOCAL_USER}":wheel "${mount_point}"
  chmod g+w "${mount_point}"

  MOUNTS+=("${mount_point}")
done
fi
EOF

status "Running SSD mount script"
bash "${RC_LOCAL}"


SHM_SIZE=$(cat /proc/meminfo | grep "MemTotal:" | awk '{m1 = $2/1048576*0.95; m2 = $2/1048576 - 10; if (m1 > m2) {m3 = m1} else {m3 = m2}; printf "%d\n", m3}')
if [ "${SHM_SIZE}" -eq 0 ]; then
  SHM_SIZE=1
fi
status "Updating /dev/shm"
{
  echo -e "\n# Set /dev/shm to 95% of memory"
  echo "tmpfs     /dev/shm     tmpfs     defaults,size=${SHM_SIZE:-500}g     0 0 "
} >>/etc/fstab
mount -o remount /dev/shm

status "DNS search qualifier"
echo "search local" >>/etc/resolv.conf

status "Updating /etc/hosts"
echo "192.168.0.5 ci.rapidsdata.com" >>/etc/hosts

status "Setting IGMP to v2"
{
echo -e "\n# Enable extra ephemeral ports\nnet.ipv4.ip_local_port_range = 20000 65500"
echo -e "\n# Enable Multicast on AWS"
} >>/etc/sysctl.conf

mapfile -t netDevices < <(ip -o -4 addr | awk '{print $2}')
for device in "${netDevices[@]}"; do
  sysctl net.ipv4.conf."${device}".force_igmp_version=2
  echo "net.ipv4.conf.${device}.force_igmp_version=2" >>/etc/sysctl.conf
done

echo "" >>/etc/sysctl.conf

status "/opt permissions"
chown root:wheel /opt
chmod g+w /opt

status "Installing AWS CLI"
if ! which aws; then
  declare -a cmd
  cmd=("yum" "install" "-y" "curl" "wget" "unzip")
  if ! retryCommand "${MAX_YUM_RETRIES}" "${cmd[@]}"; then
    error "Could not download and install curl, wget and unzip"
    exit 1
  fi

  cmd=("curl" "-L" "--fail" "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" "-o" "awscliv2.zip")
  if retryCommand "${MAX_NET_RETRIES}" "${cmd[@]}"; then
    unzip awscliv2.zip
    ./aws/install
  else
    error "Could not download AWS CLIv2"
    exit 1
  fi
fi

status "Installing perl"
yum install -y perl

status "Installing TexLive"
(
cd /usr/local
aws s3 cp "s3://rapidsse-build-support/texlive/texlive.2022.${OS_DIST}.tar.gz" - | tar xz
chown -R root:root /usr/local/texlive
chmod -R g-w /usr/local/texlive

{
  echo -e '\nTEXLIVEDIR="/usr/local/texlive"'
  echo 'PATH="${PATH}:${TEXLIVEDIR}/bin/x86_64-linux"'
  echo 'MANPATH="${MANPATH}:${TEXLIVEDIR}/texmf-dist/doc/man"'
  echo 'INFOPATH="${INFOPATH}:${TEXLIVEDIR}/texmf-dist/doc/info"'
  echo -e '\nexport PATH MANPATH INFOPATH\n'
} >>/etc/profile.d/sh.local

) &> "${TEXLIVE_OUT}" &

status "Installing system updates"
yum update -y
status "Installing epel"
yum install -y epel-release
status "Installing endpoint repo"
echo "exclude=mirror.es.its.nyu.edu" >> /etc/yum/pluginconf.d/fastestmirror.conf  # mirror doesn't respond
yum install -y https://packages.endpointdev.com/rhel/7/os/x86_64/endpoint-repo.x86_64.rpm
status "Installing gcc, git, et al"
yum install -y gcc-c++ git git-lfs libuuid-devel unzip openssl-devel python3 perl-Digest-MD5 gawk vim-enhanced perl-Digest-MD5 perl-Env java-11-openjdk zip
yum install -y centos-release-scl
yum install -y devtoolset-11-gcc devtoolset-11-gcc-c++ devtoolset-11-libatomic-devel

status "Installing cmake"
(
CMAKE_VER="3.29.0"
CMAKE_PLATFORM="linux-x86_64"
CMAKE_FILE="cmake-${CMAKE_VER}-${CMAKE_PLATFORM}.sh"
CMAKE_URL="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VER}/${CMAKE_FILE}"
CMAKE_INSTALL_DIR="/opt/cmake"

cd /opt
mkdir -p "${CMAKE_INSTALL_DIR}"

cmd=("curl" "-L" "--fail" "${CMAKE_URL}" "-o" "${CMAKE_FILE}")

if retryCommand "${MAX_NET_RETRIES}" "${cmd[@]}"; then
  sh "${CMAKE_FILE}" --prefix="${CMAKE_INSTALL_DIR}" --skip-license --exclude-subdir
  chown -R "${OS_USER}:${OS_USER}" "${CMAKE_INSTALL_DIR}"
  rm -f "${CMAKE_FILE}"

  # shellcheck disable=SC2016
  echo -e 'export PATH="${PATH}:/opt/cmake/bin"\n' >>/etc/profile.d/sh.local

else
  error "Could not download CMake" && exit 1
fi
)

status "Shellcheck"
(
cd /usr/local/bin
declare -a cmd
SHCHK_FILE="shellcheck-stable.linux.x86_64.tar.xz"
cmd=("curl" "-L" "--fail" "https://github.com/koalaman/shellcheck/releases/download/stable/${SHCHK_FILE}" "-o" "${SHCHK_FILE}")

if retryCommand "${MAX_NET_RETRIES}" "${cmd[@]}"; then
  tar "${SHCHK_FILE}" --strip-components=1 shellcheck-stable/shellcheck
  chown root:root shellcheck
  rm -f "${SHCHK_FILE}"
else
  error "Could not download ShellCheck" && exit 1
fi
)

status "Installing Sphinx"
sudo -H -u "${OS_USER}" python3 -m pip install -U --user sphinx

status "Git LFS init"
sudo -H -u "${OS_USER}" git lfs install

status "Vim preferences"
vimrc="${OS_HOME}/.vimrc"
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

status "Bash preferences"
bash_profile="${OS_HOME}/.bash_profile"
echo -e '\nexport PS1="\[\e[0;33m\]\u\[\e[0m\]@\[\e[0;32m\]\h\[\e[0m\]:\[\e[38;5;033m\]\W\[\e[0m\]\$ "\n' >>"${bash_profile}"
chown "${OS_USER}:${OS_USER}" "${bash_profile}"

cat <<EOF >>"${RC_LOCAL}"

echo "Completed the cloud-init setup!" > "${CI_STATUS}"
echo "\$(date) Completed the cloud-init setup!" > "${CI_COMPLETE}"
echo "\$(date) Completed the cloud-init setup!" > "${CI_COMPLETE_DEP}"
echo "\$(date) Rebooted" >> "${CI_OUTPUT}"
EOF

echo "System needs to be rebooted to apply changes." >> "${CI_OUTPUT}"

status "Rebooting now"
reboot
} 2>&1 | "${TEE_CMD[@]}"