# Using Git for Your Project

For the BareMetalOS project, you will be doing group development.
Therefore, we'll be asking you to use a source control system called
git.  You've already used git to checkout code/homework from the
CSSE332 repo, but for this project you will be managing a repo and
also using it to submit your homework.

# Step 1: Forking My Repo

*This step should only be done by one member of your team.*

1. Go to this URL:

   https://dev.azure.com and select "Sign in to Azure Devops"

   It should require you to login with your Rose credentials etc.
2. Click "Create Project" with some appropiate CSSE332 type name
3. Make the Project private
4. Click Create
5. Press the Blue Invite Button
6. Add all your team members plus the following course personnel

   * Michael Hewner
   * JP Mellor
   * Aaron Bartee
   * Adit Suvarna
   * Ruojie Liu

4. Click on "Repos" AND click the "SSH" button. That should give you
   your repo's URL - it should look something like this:
   
        git@ssh.dev.azure.com:v3/hewner/CSSE332%20Test%20Project/CSSE332%20Test%20Project
   
   Go to this spreadsheet and set the URL for your team
   
   https://docs.google.com/spreadsheets/d/1LOndSGkmqj_sQTuEUWwDuUL4jryyhnDVn2nrHbQcAfc/edit?usp=sharing

# Step 2: Checking out the Repo

This step should be done by everyone in your team.

1. In your browser, go to https://dev.azure.com and verify that you
   have access to your project select repos and then click SSH
2. Below the repo URL, you should see a link saying "Learn more about
   SSH" click that and then "Set up SSH key authentication".
3. Following the instructions there should walk you through creating
   some keys, adding them to your account, and then checking out your
   (probably empty repos).
   
# Step 4: Commiting the starter code

*This step should only be done by one member of your team.*

1. Get the InputOutput directory from the course repository and copy
   it into your repo
2. Add the new directory by saying
   
        git add InputOutput
        
3. Make a commit with your new changes

        git commit -m "some message (maybe initial code)"
        
4. To push to the shared repo

        git push

5. If everyone on your team does a

        git pull
        
they should get all the new changes.

To minimize merge conflicts, always pull before you start development
and push as soon as your done.  Avoid editing files at the same time
other members are.
