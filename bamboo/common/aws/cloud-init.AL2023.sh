#!/bin/bash
# shellcheck disable=SC2164

# This is a cloud-init script that can be used when starting a new EC2 instance with AmazonLinux2023.

CLOUD_INIT_OUTPUT="/cloud-init-output.log"
CLOUD_INIT_STATUS="/cloud-init-status.log"
CLOUD_INIT_COMPLETE="/cloud-init-complete.log"
CLOUD_INIT_COMPLETE_DEPRECATED="/tmp/cloud-init-complete.log"

TEXLIVE_OUTPUT="/cloud-init-texlive.log"
TEE_CMD=("tee" "-ia" "${CLOUD_INIT_OUTPUT}")
MAX_NETWORK_RETRIES=10
MAX_DNF_RETRIES=3
RC_LOCAL="/etc/rc.d/rc.local"


# ----------------------------------------------------------------------
# Functions
# ----------------------------------------------------------------------

# Writes a status message to the console as well as to a defined status file
# so that the bamboo log can inspect this file periodically as the machine boots.
function status() {
  # Write it to the status file
  echo "$@" > "${CLOUD_INIT_STATUS}"

  # Also print it to the std output
  echo
  echo "$@"
  echo
}

# Writes an error message to stderr as well as to a defined status file
# so that the bamboo log can inspect this file periodically as the machine boots.
function error() {
    # Write it to the status file
  echo "$@" > "${CLOUD_INIT_STATUS}"

  # Also print it to the std error
  echo >&2 "****** Cloud init error *******"
  echo >&2 "$@"
  echo >&2 "****** Cloud init error *******"
}


# A function that retries a command until it returns a successful exit code.
# Each subsequent attempt is separated by a 1 second delay.
# Params:
#  $1 : maximum number of retries
#  $@ : The command to execute, as an array
#
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
# ----------------------------------------------------------------------


# We wrap everything in a subshell to redirect all output through the tee command
# and to a file of our choosing.
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
  status "Starting the cloud-init setup script."

  PLATFORM="AL2023"
  OS_USER="ec2-user"
  OS_HOME_DIR="/home/${OS_USER}"
  ARCH="$(uname -p)" # should be "x86_64" or "aarch64"

  readonly PLATFORM
  readonly OS_USER
  readonly OS_HOME_DIR
  readonly ARCH

  if [ -f /etc/system-release ]; then
    OS_VER=$(cat /etc/system-release)

    if ! [[ "${OS_VER}" =~ 'Amazon Linux release 2023' ]]; then
      error "Expected the platform identification to be \"Amazon Linux release 2023\"; Got \"${OS_VER}\"."
      exit 1
    fi
  fi

  # ----------------------------------------------------------------------------------------
  # Set up SSH. We do this early so that we can still login if something goes wrong later on.
  status "*** Installing a common internal SSH key ***"

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
  status "*** Setting up the rc.local file ***"
  if [ ! -f "${RC_LOCAL}" ]; then
    {
      echo "#!/bin/sh -e"
      echo
      echo
    } > "${RC_LOCAL}"
  fi
  chmod +x "${RC_LOCAL}"


  # ----------------------------------------------------------------------------------------

  # Partition, format and mount the instance stores if they are not already mounted.
  # This needs to be done whenever the instance is stopped, but not when it is rebooted.

  status "*** Installing the SSD mount script ***"
  cat <<'EOF' >>"${RC_LOCAL}"

# ----------------------------------------------------------------------------------------
# Setup the SSDs on each boot

LOCAL_USER="ec2-user"

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
  status "*** Running the SSD mount script ***"
  bash "${RC_LOCAL}"


  # ----------------------------------------------------------------------------------------
  # Give more memory to /dev/shm. Use 95% of system memory, or mem-10GB, whichever is larger.
  SHM_SIZE=$(grep "MemTotal:" < /proc/meminfo | awk '{m1 = $2/1048576*0.95; m2 = $2/1048576 - 10; if (m1 > m2) {m3 = m1} else {m3 = m2}; printf "%d\n", m3}')
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

  # ----------------------------------------------------------------------------------------
  # DNS (Route 53) resolution
  # This will force unqualified names to tried with the .local domain name and
  # then be looked up in DNS.
  status "*** Setting DNS domain name search qualifier ***"
  echo "search local" >>/etc/resolv.conf

  # ----------------------------------------------------------------------------------------
  # /etc/hosts shortcuts
  status "*** Adding 192.168.0.5 as a shortcut for ci.rapidsdata.com to /etc/hosts ***"
  echo "192.168.0.5 ci.rapidsdata.com" >>/etc/hosts
  # Otherwise it costs us money when we transfer data via the elastic IP associated with
  # this URL.

  # ----------------------------------------------------------------------------------------
  # Setting IGMP to v2 instead of v3 (for multicast)
  status "*** Setting IGMP to v2 ***"

  # First get the network device name
  NIC="$(ip -o -br link show | awk '{print $1}' | grep -v "lo")"

  sysctl "net.ipv4.conf.${NIC}.force_igmp_version=2"
  {
    echo
    echo "# Enable Multicast on AWS"
    echo "net.ipv4.conf.${NIC}.force_igmp_version=2"
    echo
  } >>/etc/sysctl.conf

  # ----------------------------------------------------------------------------------------
  # Change permissions to allow for writing to /opt
  status "*** Setting /opt permissions ***"
  chown root:wheel /opt
  chmod g+w /opt


  # ----------------------------------------------------------------------------------------
  # Installing AWS CLI
  # Note that we retry any network operations as these can fail or timeout for
  # various reasons. A timeout means that we ultimately cannot connect to this
  # machine.
  status "*** Installing AWS CLI ***"

  if ! which aws; then
    # AWS CLI needs curl and unzip
    declare -a cmd
    cmd=("dnf" "install" "-y" "curl" "wget" "unzip")
    if ! retryCommand "${MAX_DNF_RETRIES}" "${cmd[@]}"; then
      error "ERROR: Could not download and install curl, wget and unzip - the dnf repository may be "\
            "unavailable or a network error could be preventing access."
      exit 1
    fi

    cmd=("curl" "-L" "--fail" "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" "-o" "awscliv2.zip")
    if retryCommand "${MAX_NETWORK_RETRIES}" "${cmd[@]}"; then
      unzip awscliv2.zip
      ./aws/install
    else
      error "ERROR: Could not download AWS CLIv2 - the version might be old and may need to be updated "\
            "or a network error could be preventing access."
      exit 1
    fi
  fi

  # ----------------------------------------------------------------------------------------
  # Install Tex Live in parallel in the background to /usr/local/texlive.
  # This is for generating PDF documentation.
  status "*** Installing TexLive ***"
  (
    cd /usr/local
    aws s3 cp "s3://rapidsse-build-support/texlive/texlive.2023.${PLATFORM}.${ARCH}.tar.gz" - | tar xz
    chown -R root:root /usr/local/texlive
    chmod -R g-w /usr/local/texlive

    echo
    status "Setting up TexLive environment variables"
    {
      echo
      echo 'TEXLIVEDIR="/usr/local/texlive"'
      # shellcheck disable=SC2016
      echo 'PATH="${PATH}:${TEXLIVEDIR}/bin/$(uname -p)-linux"'
      # shellcheck disable=SC2016
      echo 'MANPATH="${MANPATH}:${TEXLIVEDIR}/texmf-dist/doc/man"'
      # shellcheck disable=SC2016
      echo 'INFOPATH="${INFOPATH}:${TEXLIVEDIR}/texmf-dist/doc/info"'
      echo
      echo 'export PATH MANPATH INFOPATH'
      echo
    } >>/etc/profile.d/sh.local

    echo
    status "TexLive installation complete"
  ) > "${TEXLIVE_OUTPUT}" &

  # ----------------------------------------------------------------------------------------
  # Apply any package updates
  status "*** Installing updates and necessary packages ***"
#  amazon-linux-extras install -y epel
  status "*** Doing a dnf update ***"
  dnf update -y --releasever=latest

  status "*** Installing gcc, git, et al ***"
  dnf install -y gcc-c++ git git-lfs java-11-amazon-corretto-headless cmake libuuid-devel libatomic-static unzip nmap-ncat ncurses openssl-devel parallel python3 python3-pip perl-Digest-MD5 perl-Env perl-File-Copy perl-Unicode-Normalize perl-sigtrap perl-Time-HiRes zip

  status "*** Installing other useful utilities ***"
  dnf install -y nethogs iftop iperf3 iotop jq telnet
  # Not supported: nload bmon

  # ShellCheck
  status "*** Shellcheck ***"
  (
    cd /usr/local/bin

    declare -a cmd
    cmd=("curl" "-L" "--fail" "https://github.com/koalaman/shellcheck/releases/download/stable/shellcheck-stable.linux.x86_64.tar.xz" "-o" "shellcheck-stable.linux.x86_64.tar.xz")

    if retryCommand "${MAX_NETWORK_RETRIES}" "${cmd[@]}"; then
      tar xvJf shellcheck-stable.linux.x86_64.tar.xz --strip-components=1 shellcheck-stable/shellcheck
      chown root:root shellcheck
      rm -f shellcheck-stable.linux.x86_64.tar.xz
    else
      error "ERROR: Could not download ShellCheck - the version might be old and may need to be updated "\
            "or a network error could be preventing access."
      exit 1
    fi
  )

  # ----------------------------------------------------------------------------------------
  # Install JMeter to /opt/jmeter
  status "*** Installing jmeter ***"

  (
    cd /opt
    JMETER_VER="5.6.3"
    JMETER_NAME="apache-jmeter-${JMETER_VER}"
    JMETER_FILE="${JMETER_NAME}.tgz"

    declare -a cmd
    cmd=("curl" "-L" "--fail" "https://archive.apache.org/dist/jmeter/binaries/${JMETER_FILE}" "-o" "${JMETER_FILE}")

    if retryCommand "${MAX_NETWORK_RETRIES}" "${cmd[@]}"; then
      tar xvzf "${JMETER_FILE}"
      rm -f "$JMETER_FILE"
      ln -s "${JMETER_NAME}" jmeter
      chown -R ec2-user:ec2-user "${JMETER_NAME}"
      chown ec2-user:ec2-user jmeter
    else
      error "ERROR: Could not download JMeter - the version might be old and may need to be updated "\
            "or a network error could be preventing access."
      exit 1
    fi
  )

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
  status "*** Installing Sphinx ***"
  sudo -H -u "${OS_USER}" python3 -m pip install --user -U sphinx

  # ----------------------------------------------------------------------------------------
  # Setting JAVA_HOME
  status "*** Setting JAVA_HOME ***"
  cat <<EOF >/etc/profile.d/java_home.sh
#!/bin/bash
export JAVA_HOME="$(readlink -f /usr/bin/java | sed "s:/bin/java::")"
EOF

  # ----------------------------------------------------------------------------------------
  # Spot instance interruption notification
  status "*** Spawning spot instance interruption notification ***"

  # References:
  #  https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/spot-interruptions.html#spot-instance-termination-notices
  #  https://aws.amazon.com/blogs/compute/best-practices-for-handling-ec2-spot-instance-interruptions/
  #  https://stackoverflow.com/questions/25282661/how-do-i-broadcast-messages-to-all-bash-terminal-in-python-using-wall-command-wi

  SPOT_NOTIFIER="/spotInterruptionNotifier.sh"
  SPOT_NOTIFIER_OUTPUT="/spotInterruptionNotifier.nohup"

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

  # Add this spot notifier to run on every reboot
  echo "sudo -H -u ${OS_USER} nohup ${SPOT_NOTIFIER} 2>&1 | sudo -H -u ${OS_USER} tee ${SPOT_NOTIFIER_OUTPUT} > /dev/null &" >> "${RC_LOCAL}"

  # Now run it
  sudo -H -u ${OS_USER} nohup "${SPOT_NOTIFIER}" 2>&1 | sudo -H -u ${OS_USER} tee "${SPOT_NOTIFIER_OUTPUT}" > /dev/null &

  # ----------------------------------------------------------------------------------------
  # Vim annoyances
  status "*** Setting vim preferences ***"

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
  status "*** Setting bash preferences ***"

  bash_profile="${OS_HOME_DIR}/.bash_profile"
  {
    echo "";
    echo '#export PS1="[\u@\h \W]\$ ';
    # shellcheck disable=SC2028
    echo 'export PS1="\[\e[0;33m\]\u\[\e[0m\]@\[\e[0;32m\]\h\[\e[0m\]:\[\e[38;5;033m\]\W\[\e[0m\]\$ "';
    echo "";
  } >> "${bash_profile}"

  # ----------------------------------------------------------------------------------------
  # Git setup
  status "*** Git LFS initialisation ***"
  sudo -H -u "${OS_USER}" git lfs install

  # ----------------------------------------------------------------------------------------
  # Remove annoying GNU Parallel citation message
  status "*** GNU Parallel citation ***"
  if which parallel &>/dev/null; then
    echo -e "will cite\n" | parallel --citation
  fi

  # ----------------------------------------------------------------------------------------
  # Install a script that will indicate that the cloud-init is complete after rebooting
  cat <<EOF >>"${RC_LOCAL}"

echo "Completed the cloud-init setup script!" > "${CLOUD_INIT_STATUS}"
echo "\$(date) Completed the cloud-init setup script!" > "${CLOUD_INIT_COMPLETE}"
echo "\$(date) Completed the cloud-init setup script!" > "${CLOUD_INIT_COMPLETE_DEPRECATED}"
echo "\$(date) Rebooted" >> "${CLOUD_INIT_OUTPUT}"
EOF

  {
    echo "*********************************************"
    echo "Completed the cloud-init setup script!"
    echo "System needs to be rebooted to apply changes."
    echo "*********************************************"
    echo
  } >> "${CLOUD_INIT_OUTPUT}"

  # ----------------------------------------------------------------------------------------
  # Reboot to ensure all these changes take effect
  status "*** Rebooting now ***"
  reboot

} 2>&1 | "${TEE_CMD[@]}"