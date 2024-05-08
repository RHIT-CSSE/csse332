#
# Last updated: Wed 22 Nov 2023
#

# configuration options, adjust as you see fit
SHELL=bash
ACTERM=2324d
USERNAME=noureddi
REMOTE_DIR=addiator.rose-hulman.edu:/class/csse/csse332/${ACTERM}

all: serve

.PHONY: clean all rsync

serve:
	bundle exec jekyll serve -q -l -o

background:
	tmux new -d -s server "bundle exec jekyll serve -q -l -o"

restart: kill background

kill:
	tmux kill-session -t server || true

build:
	bundle exec jekyll build

www: clean
	@echo -e '\033[1;mBuilding the webpage\033[0m'
	bundle exec jekyll build -q
	@echo -e '\033[1;32mCopying to remote server\033[0m'
	cd ./_site/ && rsync -e ssh -Paz --delete . ${USERNAME}@${REMOTE_DIR} && cd -
	@echo -e '\033[1;32mDone\033[0m'

rsync:
	@echo -e '\033[1;32mCopying _site directory to remote server\033[0m'
	cd ./_site/ && rsync -e ssh -Paqz --delete . ${USERNAME}@${REMOTE_DIR} && cd -
	@echo -e '\033[1;32mDone\033[0m'

echo:
	@echo -e 'TERM is ${ACTERM}'
	@echo -e 'USERNAME is ${USERNAME}'
	@echo -e 'Remote target is ${REMOTE_DIR}'

clean: kill
	bundle exec jekyll clean

