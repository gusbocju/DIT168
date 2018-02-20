## HOW TO CONTRIBUTE TO THIS PROJECT
In order to contribute, pick an issue describing a bug or a desired feature from the integrated project board on GitHub and create a new branch for your contribution. The branch name format should be a *noun* or an *adjective_noun*, followed by *_fix* for a bug solution (e.g. `parallel_parking`, `driving`, `car_detecting_fix`).\
\
While developing your contribution, commit frequently and include a descriptive message each time (i.e. *no* `fixing stuff`, *but instead* `refactoring variable names`, or `correcting an accidental endless loop`). When you have completed your addition of code, create a pull request to initiate the peer review and eventually make your contribution to master. Your pull request has to be reviewed by at least one accredited contributor before it'll be merged into master.

All new functionalities and additions to existing code should be commented properly. There should be a description over each function, preferably in `/* ... */` style, and it should follow the template described below:

/*
	Input: <input, in natural language><br />
	Output: <output, in natural language><br />
	==========<br />
	<short description; optional, but preferable>
*/<br />

Example comment:<br />
/*	
	Input: two integers, which are comparable. No context needed.<br />
	Output: the bigger of the two integers.<br />
	==========<br />
	This method returns the bigger of two integers.
*/<br />

### ADDITIONAL GUIDELINES TO KEEP IN MIND:
* Warnings shall be treated as errors.<br />
* Keep the highest possible warning levels.<br />
* Keep your code platform independent (stick to the C++ standard libraries).<br />
* Stick to the the C++14 standard.<br />


## HOW TO PACKAGE AND RELEASE BINARIES OF THIS SOFTWARE
We use [Docker](https://www.docker.com/community-edition) to build and deploy images containing any features and/ or modules relevant to the project - and encourage you to do the same. The latest image can be fetched from `gusbocju/dit168.marble:latest` on Docker Hub and is automatically maintained through Travis CI.

Please refer back to our [README.md](README.md) for additional information on how building the project or running the code is actually done, respectively [https://docs.docker.com](https://docs.docker.com) for any other Docker related matter.
