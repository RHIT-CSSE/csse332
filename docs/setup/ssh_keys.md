---
title: Setting up ssh keys
layout: post
readtime: true
date: Mon 02 Dec 2024
last-updated: Mon 02 Dec 2024
---

# Table of contents

# The problem

If you are getting authentication issues when trying to clone or update **your
own private** repository, then you will need to add your ssh keys to your
GitHub account.

For more detailed information, please see the article [here](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)
and [here](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account), but here's a quick and dirty guide for setting things up.

## Generating your keypair

To generate your public/private keypair, you will need to start from a
terminal window **on your Linux machine** (doing this from Gitbash or
Powershell will not work!).

```shell
$ ssh-keygen
```

Accept all the defaults and enter a passphrase if you'd like to use one (you
can leave it blank if you'd prefer not to). When the generation completes, two
new files will show up under your `.ssh/` directory under your home directory.

You can check its contents using:

```shell
$ ls ~/.ssh/
```

You should see two new files, depending on your system, they are `id_rsa` and
`id_rsa.pub`, or `id_ed25519` and `id_ed25519.pub`.

Finally, copy your **public** key (i.e., the one that ends with a `.pub`), you
can display it on the screen using:

```shell
$ cat ~/.ssh/id_ed25519.pub
```

(or replace with `id_rsa.pub` if that is your case). You can simply type `cat
~/.id_` and then press the `<tab>` key and the terminal will autocomplete it
for you. Then complete with the `.pub` suffix.

## Add your key to GitHub

Next, open up a browser window and navigate to the GitHub homepage. Open your
account settings by clicking on the user icon in the upper-right corner and
then choose settings from the drop down menu.

On the settings page, select _SSH and GPG Keys_ from the menu on the left and
click the _New SSH Key_ green button.

Give the key a name (e.g., Ubuntu wsl2 key) and then paste the key you copied
from above into the large text box. Finally, click the _Add SSH Key_ button.

## Notes

Please note that even if you generate and add your keys, you will not be able
to write to the class repository. You only have write-access to your own
private repository, so make sure to **read** and follow the instructions for
each lab to make sure you are working on the right stuff.

---

This page was last edited by Mohammad Noureddine on {{ page.last-updated }}. If
you notice any typos or inaccuracies, please open a GitHub issue on this
[repository]({{site.gh_repository_url}}).

