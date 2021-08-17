#!/bin/bash

comm -23 <(git log --oneline | awk '/Apply patch/{print $4}' | sort) <(git log --oneline | awk '/Remove patch/{print $4}' | sort)
