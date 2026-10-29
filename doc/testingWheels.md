# testing a cloudComPy wheel

Until the wheel for your system and Python version becomes available on the PyPI servers, you can download the wheel from the [cloudComPy website](https://www.simulation.openfields.fr/index.php/projects/cloudcompy). Select the wheel that matches your operating system and Python version.

To test the wheel without risking damage to anything in your environment, the best solution is to create a Python environment (venv).
For example, if you're using Python 3.10 and it's already installed:

```
/path/to/python3.10 -m venv venv310
source venv310/bin/activate
python3 -m pip install --upgrade pip
pip install /path/to/cloudcompy-2.14.0-cp310-cp310-macosx_13_0_arm64.whl
```

As soon as the wheel is available on the PyPI servers, you can use the `pip install cloudComPy` command shown in the instructions above.
