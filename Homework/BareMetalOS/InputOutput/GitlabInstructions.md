# Using Git for Your Project

For the BareMetalOS project, you will be doing group development.
Therefore, we'll be asking you to use a source control system called
git.  You've already used git to checkout code/homework from the
CSSE332 repo, but for this project you will be managing a repo and
also using it to submit your homework.

# Step 1: Creating a repo

*This step should only be done by one member of your team.*

1. Go to this URL:

   https://ada.csse.rose-hulman.edu/projects/new

   It should require you to login with your Rose credentials etc.
2. Give the project with some appropiate CSSE332 type name
3. Make the Project private
3. Initialiize the repo with a readme (just so you know when you check it out)
4. Click "Create project"
5. Click the gear in the left sidebar and select "Members"
6. Add all your team members plus the following course personnel

   * Michael Hewner
   * Max Wang
   * Steven Crowell
   * Taylor Chatfield

as Maintainers.  Note that your team members will need to have logged
onto ada at least once so their name appears in the list.

4. Click on the home icon in the left sidebar and select "Clone".  That should give you your repos URL (pick the SSH one) - it should look something like this:
   
        git@ada.csse.rose-hulman.edu:hewner/test-project-qqq.git
   
   Go to this spreadsheet and set the URL for your team
   
   https://docs.google.com/spreadsheets/d/1chyis63Cqf9-3Q1HtroPyHVzJ9FcnBQ4TlHAZHv40wg/edit?usp=sharing

   BE SURE YOU USE YOUR CORRECT TEAM NUMBER!  It should be in the CATME email your received.

# Step 2: Checking out the Repo

This step should be done by everyone in your team.

You'll probably find it easiest to use HTTPS authentication.  If you know how to set up SSH credentials you can use those too.

1. Get the HTTPS link from the clone button.  It should look something like this:

        https://ada.csse.rose-hulman.edu/hewner/test-project-qqq.git

2. Go to your local linux command line and type:

        git clone <HTTPS LINK>
        
   it should ask you for your RH credentials and then create a
   directory named the same as your project.
   
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
