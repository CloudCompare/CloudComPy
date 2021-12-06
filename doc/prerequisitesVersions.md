## prerequisites required versions

The minimum required version of each prerequisite is not always precisely identified. Examples of constructions that work are given here.

First example: Linux64, Ubuntu 20.04, all native packages.

Second and third example: Windows 10, Visual Studio 2019, Anaconda3 or miniconda to get all the prerequisites plus a lot more. With Python 3.7, there are few combination of working packages, it's easier to use Python 3.9. Qt 5.15.2 should be installed apart (binary package), it solves a bug on xyz files reading.

Fourth example: Linux64 with conda packages, Python 3.9.

| Platform | Linux Native | Windows 10      | Windows 10      | Linux           | minimum |
| -------- | -------------|---------------- | --------------- | --------------- | ------- |
|          | Ubuntu 20.04 | conda Python3.7 | conda Python3.9 | Conda Python3.9 |         |
|          | (clang 10)   | VS 2019         | VS 2019         | (clang 10)      |         |
| -------- | -------------|---------------- | --------------- | --------------- | ------- |
| Qt       | 5.12.8       | 5.15.2          | 5.15.2          | 5.12.9          | 5.9 ?   |
| Python   | 3.8.5        | 3.7.10          |                 | 3.9.7           | 3.6     |
| Boost    | 1.71         | 1.68            |                 | 1.72.0          | 1.68 ?  |
| Numpy    | 1.17.4       | 1.20.2          |                 | 1.21.4          | 1.13 ?  |
| -------- | -------------|---------------- | --------------- | --------------- | ------- |
