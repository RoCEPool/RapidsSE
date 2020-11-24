#!/bin/bash
cd /home/ec2-user/se
scp nat:se.tar.gz .
tar xvzf se.tar.gz
cd Demo
