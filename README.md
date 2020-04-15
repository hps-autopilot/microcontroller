# Human Powered Submarine Autopilot System

Project Statement: Develop an autopilot system that could be implemented to control the roll of the human-powered submarine as it navigates through the water. This system would be composed of a prototype hardware module with a proof-of-concept vehicle orientation display, a data control loop, and the interface between hardware and software.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine.

### Prerequisites

For this project, we chose to use ARDUINO NANO 33 BLE board. For it has multiple sensors to collect various types of data at real time, and then process the data in our control algorithm.

### Installing

First make sure to have `Arduino IDE` and `git` installed on your computer.

* Click [here](https://www.arduino.cc/en/Main/Software) for `Arduino IDE` tutorial.
* Click [here](https://dev.to/landonp1203/how-to-properly-set-up-git-on-your-computer-33eo) for `git` tutorial.
å

After installing, open a destination folder of where you want to store this project. Then open terminal and input the following code:
```
git clone https://github.com/hps-autopilot/microcontroller.git
```

When you finished, go to hps_capstone  folder, hps_auto_pilot, and open up `hps_auto_pilot.ino` file on Arduino software.

In this project, we use some additional Arduino Libraries, which have to be installed manually.

The Arduino Libraries are:
* Arduino_LSM9DS1
* Arduino_LPS22HB
* Arduino_HTS221
* Arduino_BLE

Click [here](https://www.arduino.cc/en/guide/libraries) if you need help downloading and installing the libraries on Arduino IDE.  

Congratulations! You've completed the set up and READY TO ROLL!
<!--
## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags).

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc -->
