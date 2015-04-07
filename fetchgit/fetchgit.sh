#!/bin/bash

# This script will clone/fetch git repositories from GitHub
# to a local repository
# use a cronjob to automate this process

# path to local backup directory for the repos
repodir=/home/pabu/localgit
# remote repo directory address
repoaddr=https://github.com/devthrash
# add names of the repositories to be backed up to this list
repo_list="
	elbsupply
	distamp
	gist
"

echo "Fetching repos at $(date +'%Y-%m-%d %H:%M')"

# don't worry about this part anymore
cd $repodir

for rep in ${repo_list}
do
	if [ -d $rep ]
		then
			cd $rep
			git pull
		else
			git clone ${repoaddr}/$rep
	fi
	
	cd $repodir
	chown -R pabu:pabu $rep
done
