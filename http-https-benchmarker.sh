#!/bin/bash

# http
time ./http_client $1

# https
time ./https_client $1
