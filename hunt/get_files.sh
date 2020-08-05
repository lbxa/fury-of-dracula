#!/bin/bash
blacklist=(Game.h Places.c Places.h Map.c Map.h testDraculaView.c testHunterView.c testGameView.c testUtils.c testUtils.h player.c dracula.h hunter.h)

files=(Makefile)

for file in *.c
do
  # shellcheck disable=SC2199
  # shellcheck disable=SC2076
  if [[ ! " ${blacklist[@]} " =~ " ${file} " ]]; then
    # whatever you want to do when array doesn't contain value
    files+=("$file")
  fi
done

for file in *.h
do
  # shellcheck disable=SC2199
  # shellcheck disable=SC2076
  if [[ ! " ${blacklist[@]} " =~ " ${file} " ]]; then
    # whatever you want to do when array doesn't contain value
    files+=("$file")
  fi
done

#files=($(sort <<<"${files[*]}");
IFS=' ';echo "${files[*]// /|}";IFS=$' \t'
echo "$IFS"