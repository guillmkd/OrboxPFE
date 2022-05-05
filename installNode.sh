#!/usr/bin/env bash
sudo apt-get update
sudo apt-get upgrade
sudo apt install -y curl libttspico-utils
curl -sL https://deb.nodesource.com/setup_7.x | sudo -E bash -
sudo apt install -y nodejs