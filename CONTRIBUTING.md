## HOW TO CONTRIBUTE TO THIS PROJECT
In order to contribute, pick an issue describing a bug or a desired feature from the integrated project board on GitHub and create a new branch for your contribution. The branch name format should be a *noun* or an *adjective_noun*, followed by `_fix` for a bug solution (e.g. `parallel_parking`, `driving`, `car_detecting_fix`).\
\
While developing your contribution, commit frequently and include a descriptive message each time (i.e. *no* `fixing stuff`, *but instead* `refactoring variable names`, or `correcting an accidental endless loop`). When you have completed your addition of code, create a pull request to initiate the peer review and eventually make your contribution to master. Your pull request has to be reviewed by at least one other accredited contributor before it can be merged into master.

## HOW TO PACKAGE AND RELEASE THE BINARIES OF THIS SOFTWARE
We use [Docker](https://www.docker.com/community-edition) to build and deploy images containing any features and/ or modules relevant to the project. The latest image can be fetched from `gusbocju/dit168.marble:latest` on Docker Hub and is automatically maintained through Travis CI.

Please refer back to our [README.md](README.md) for additional information on building the project or running the code,\
respectively [https://docs.docker.com](https://docs.docker.com) for any other Docker related matter.
