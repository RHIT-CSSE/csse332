# Using Git for Your Project

For the BareMetalOS project, you will be doing group development.
Therefore, we'll be asking you to use a source control system called
git.  You've already used git to checkout code/homework from the
CSSE332 repo, but for this project you will be managing a repo and
also using it to submit your homework.

# Step 1: Forking My Repo

*This step should only be done by one member of your team.*

1. Go to this URL:

   https://ada.csse.rose-hulman.edu/hewner/baremetalos

   You'll have to login with your rose credentials.
2. Click "fork"
3. Select your name as the namespace
4. Go to Settings > General > Permissions
5. Change the Project Visibility to "Private" and press "Save Changes"
6. Go to Settings > Members
7. Invite the other members of your team to your project (they may
   need to login at https://ada.csse.rose-hulman.edu/ so they
   show up in the list) as Maintainers
8. Invite the other members of the grading team to your project as
   Maintainers.  The team should be
   * Michael Hewner
   * Lixing Song
   * Indresh Srivastava
   * Adit Suvarna
   * Carrie Utter
9. Find your repo URL.  It should appear beneath your project's name
   on the main project page.  It should look something like this:
   
   https://ada.csse.rose-hulman.edu/XXXXXX/BareMetalOS.git
   
   Go to this spreadsheet and set the URL for your team
   
   https://docs.google.com/spreadsheets/d/1LOndSGkmqj_sQTuEUWwDuUL4jryyhnDVn2nrHbQcAfc/edit?usp=sharing

# Step 2: Checking out the Repo

This step should be done by everyone in your team.

1. In your browser, go to https://ada.csse.rose-hulman.edu and verify
   that you have access to your project.  Copy the Repo URL.
2. Type

        git clone REPO-URL

3. In the future to get changes others have made go to the created directory and type

        git pull
        
# Step 3: Commiting

1. When you have made changes and want to commit, first use "git
   status" to see the files you've changed.
2. To add a new/changed file to the commit

        git add FILENAME
        
3. When you've added all the files, use

        git commit -m "discriptive message about the changes made in this commit"
        
4. To push to the shared repo

        git push
        
To minimize merge conflicts, always pull before you start development
and push as soon as your done.  Avoid editing files at the same time
other members are.
