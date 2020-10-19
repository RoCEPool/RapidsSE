#!/bin/bash

#########################################################################################
######                             VARIABLES                                       ######
#########################################################################################

# Set to 1 to enable debugging or 0 to turn off.
DEBUG=1

# Will be populated with the SSD mount points
declare -a MOUNTS

HOSTNAME="$(hostname)"

# Directory to install SE on the local machine
SE_DIR="/home/ec2-user/se"

#########################################################################################

echo
echo
echo
echo "*************************************"
echo "Starting the cloud-init setup script."
echo "*************************************"

# ----------------------------------------------------------------------------------------
# Debugging. All output can be seen in /var/log/cloud-init-output.log
if [ "${DEBUG}" -eq 1 ]
then
  set -x
fi

# ----------------------------------------------------------------------------------------
# Give more memory to /dev/shm
echo "tmpfs     /dev/shm     tmpfs     defaults,size=500g     0 0 " >> /etc/fstab
mount -o remount /dev/shm

# ----------------------------------------------------------------------------------------
# Change permissions to allow for writing to /opt
chown root:wheel /opt
chmod g+w /opt

# ----------------------------------------------------------------------------------------
# Apply any package updates
amazon-linux-extras install epel
yum update -y


# ----------------------------------------------------------------------------------------
# Install any additional applications
yum groupinstall "Development Tools" -y
yum install texinfo -y
yum install nload nethogs bmon iftop iperf3 -y


# ----------------------------------------------------------------------------------------
# Set up /etc/hosts using the values in HOST_NAMES and HOST_ADDRS above.
echo "10.0.0.66 nat" >> /etc/hosts


# ----------------------------------------------------------------------------------------
# Set up SSH.
# All machines have the same public/private key
SSH_DIR="/home/ec2-user/.ssh"

mkdir -p "${SSH_DIR}" || exit

cat << EOF > "${SSH_DIR}/authorized_keys"
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

cat << EOF > "${SSH_DIR}/known_hosts"
nat ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBABa2KhtIwy9fU2SR/TYNRXbbxt7x1a2DVK0kObGWvAIWkrjppPQtQNTwCoa74OvjVcBH1vy0nP4m1N+8w8XMW8=
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
# Setup the SSDs

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
  
  # Get the partition name
  partition=$(lsblk -l -n -p -oNAME "${device}" | tail -n 1)
  
  echo "Formatting partition ${partition}"  
  counter=$((counter + 1))
  mount_point="/mnt/disk${counter}"
  
  mkfs.ext4 -E nodiscard -m0 "${partition}" || exit
  
  echo "Mounting partition ${partition} to ${mount_point}"
  
  mkdir -p "${mount_point}"
  mount -o discard "${partition}" "${mount_point}" || exit
  chown -R ec2-user:ec2-user "${mount_point}"
  
  MOUNTS+=("${mount_point}")
done


# ----------------------------------------------------------------------------------------
# Vim annoyances
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
# Setup influxdb and telegraf
echo "Setting up InfluxDB and Telegraf"
cd /tmp || exit

# Download & install InfluxDB
influx_release="influxdb-1.8.2.x86_64.rpm"
wget "https://dl.influxdata.com/influxdb/releases/${influx_release}"
yum localinstall -y "${influx_release}"
rm -f "${influx_release}"

# Download & install Telegraf
telegraf_release="telegraf-1.15.2-1.x86_64.rpm"
wget "https://dl.influxdata.com/telegraf/releases/${telegraf_release}"
yum localinstall -y "${telegraf_release}"
rm -f "${telegraf_release}"

# Install collectd (necessary)
yum install collectd -y

# Configure influxdb
perl -0777 -i.orig -pe 's/(\[\[collectd\]\]\s*\n\s*)# enabled = false/\1enabled=true/igs' /etc/influxdb/influxdb.conf

# Configure telegraf
cp /etc/telegraf/telegraf.conf /etc/telegraf/telegraf.conf.orig
cat << EOF > /etc/telegraf/telegraf.conf
# Telegraf Configuration
[global_tags]

[agent]
  interval = "5s"
  round_interval = true
  metric_batch_size = 1000
  metric_buffer_limit = 10000
  collection_jitter = "2s"
  flush_interval = "10s"
  flush_jitter = "2s"
  precision = ""
  hostname = ""
  omit_hostname = false

[[outputs.influxdb]]

[[inputs.cpu]]
  percpu = true
  totalcpu = true
  collect_cpu_time = false
  report_active = false

[[inputs.disk]]
[[inputs.diskio]]
[[inputs.kernel]]
[[inputs.mem]]
[[inputs.processes]]
[[inputs.swap]]
[[inputs.system]]
[[inputs.interrupts]]
[[inputs.linux_sysctl_fs]]
[[inputs.net]]
[[inputs.netstat]]
EOF

# Start influxdb and telegraf
#systemctl start influxdb 
#systemctl start telegraf

cd ~ || exit

# ----------------------------------------------------------------------------------------
# Turn off debugging if required.
if [ "${DEBUG}" -eq 1 ]
then
  set +x
fi

echo "**************************************"
echo "Completed the cloud-init setup script!"
echo "**************************************"
echo
echo
echo