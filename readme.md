# Overview

This is the readme file belonging to the top-level folder of my bachelor thesis concerning the integration of runtime verification in the F' framework.

Here I will give an overview of how to run the example deployment in the provided docker container.

Inside the `ReelayComponents` folder is another `readme.md` that contains instructions of how to use the constructed components in an F' deployment.


# Run the Example

Prerequisites:

1. Docker installed and the docker daemon running
2. Git installed
3. at least 6 GB of empty storage space (I'm sorry!)

To run the example first clone this repository to your machine:

```bash
git clone --recurse-submodules github.com/RomanRiesen/thesis
```

Then cd into the cloned directory:

```bash
cd thesis
```

Now run

```bash
docker build -t "fprime_with_reelay" .
```

to construct the docker image, which will take a few minutes (easily ten or more). The resulting image will als be pretty big at around 2.5 GB.
After the successful completion of the image generation we can run it:

```bash
docker run -it -p 5000:5000 fprime_with_reelay
```

Now you should be able to navigate to 127.0.0.1:5000 in a web browser and then see the fprime ground station.

You can now, for example, click on the tab `Charts`, then in the top-right click on `Load` to load the `rocket_charts.txt` chart definitions from `components_example/Ref/`. This will load a few charts. Then go over to the `Commanding` tab, click the drop-down, search for `toggle` and execute the command `rocketSimulation.TOGGLE_SIMULATION()`. Now you can observe the rocket simulation values in the `Charts` tab and the events generated from the monitor inside the
`Events` tab.
