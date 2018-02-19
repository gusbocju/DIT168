![build status on #master:](https://travis-ci.org/gusbocju/DIT168.svg?branch=master)

## DIT168 V18 Project: Industrial IT and Embedded Systems

During the spring term of 2018 - and as part of DIT168 V18 at the University of Gothenburg, this repository will be used to design and implement software for a mini car to drive autonomously under stable conditions. The car should be able to, in addition to self-driving, follow a line, do safe handling of intersections, pass other vehicles and park. To accomplish this the car is equipped with sensors. The project will be done in C/C++ and built upon [OpenDaVINCI](http://opendavinci.cse.chalmers.se/www/) - with a high focus on quality assurance, effective design and testing throughout. 

### GIT LAYOUT:

Please keep individual branches per feature in mind; they’ll be merged using pull requests that have to approved by at least one other contributor/ group member. There shall be no direct pushes to the master branch; prototypes will only evolve by pulling completed features into the release branch. In addition to the feature branches, each contributor creates a separate branch from the feature branch for which they push their code to.\
\
Following the original folder structure, please make sure to place all generated code and/ or supporting files within the right directory - which, in general means: 
Within the `src/` folder for anything code related; within additional sub-directories (keeping precise naming in mind) per feature (e.g. `src/networking/` for anything network related); and within supporting `test/` and `demo/` sub-directories for inherent demos and tests accordingly.

### CLONING
`$ git clone https://github.com/gusbocju/DIT168.git`

### BUILD AND DEPLOY
`$ docker build -t dit168.marble -f Dockerfile .`

(successful builds originating master will be pushed to `gusbocju/dit168.marble:latest` on Docker Hub using Travis CI)

#### SAVE AND RESTORE:
`$ docker save dit168.marble > dit168.marble.tar`\
`$ cat dit168.marble.tar | docker load`

#### PULL:
`$ docker pull gusbocju/dit168.marble`

#### RUN:
`$ docker run --rm -ti --net=host dit168.marble /opt/MARBLE`

#### DEMOS:
`$ docker run --rm -ti --net=host dit168.marble /opt/MARBLE.Networking.Demo.Sender`\
`$ docker run --rm -ti --net=host dit168.marble /opt/MARBLE.Networking.Demo.Receiver`\
\
(the receiving process has to be terminated using `$ docker sp` and `$ docker stop <container id>`)

### CONTRIBUTING:
Please read the [CONTRIBUTING.md](CONTRIBUTING.md) on how to contribute to this project.

### LICENSE:
This project is licensed under MIT- see [LICENSE.md](LICENSE.md) for more details.





