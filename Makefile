all: serve

.PHONY: all

serve:
	bundle exec jekyll serve

background:
	tmux new -d -s server "bundle exec jekyll serve"

kill:
	tmux kill-session -t server
