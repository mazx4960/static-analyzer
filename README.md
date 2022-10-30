[![codecov](https://codecov.io/gh/nus-cs3203/22s1-cp-spa-team-14/branch/master/graph/badge.svg?token=O0pTcTxSWs)](https://codecov.io/gh/nus-cs3203/22s1-cp-spa-team-14)
[![Linux Build](https://github.com/nus-cs3203/22s1-cp-spa-team-14/actions/workflows/build_linux.yml/badge.svg)](https://github.com/nus-cs3203/22s1-cp-spa-team-14/actions/workflows/build_linux.yml)
[![Windows Build](https://github.com/nus-cs3203/22s1-cp-spa-team-14/actions/workflows/build_windows.yml/badge.svg)](https://github.com/nus-cs3203/22s1-cp-spa-team-14/actions/workflows/build_windows.yml)

# Team 14

## Target Environment

Item | Version
-|-
OS | Windows 10
Toolchain | Microsoft Visual Studio Enterprise 2019 Version 16.11.0
C++ Standard | C++17

### Additional Build Instructions

Building the project:

```bash
scripts/build.sh
```

Running tests:

```bash
scripts/test.sh
```

Running the program:

```bash
scripts/run.sh
```

### Profiling statistics

```bash
scripts/profile.sh
```

Results:

```
==============================================================
Profiling 'compute_centroid'
[MEM USAGE] Peak Memory Usage: 6.992 MiB
[PARSE TIME] 0.668000ms
[MAX QUERY TIME] 1.643000ms
==============================================================
==============================================================
Profiling 'sp_stress_1'
[MEM USAGE] Peak Memory Usage: 9.004 MiB
[PARSE TIME] 14.345000ms
[MAX QUERY TIME] 1.746000ms
==============================================================
==============================================================
Profiling 'stress_1'
[MEM USAGE] Peak Memory Usage: 230.609 MiB
[PARSE TIME] 10.792000ms
[MAX QUERY TIME] 2041.427000ms
==============================================================
==============================================================
Profiling 'stress_2'
[MEM USAGE] Peak Memory Usage: 252.105 MiB
[PARSE TIME] 9.016000ms
[MAX QUERY TIME] 1577.948000ms
==============================================================
```

# Team Members

Name | Mobile | Email | Development OS/Toolchain
-:|:-:|:-|-|
Chen Xihao | 96376285 | chenxihao@u.nus.edu | MacOS 12 / CLion
Desmond Tan Jing Shen | 90481763 | desmondtanjs@u.nus.edu | MacOS 12 / CLion
Soo Wei Kang Kelvin | 96605926 | kelvin.soo@u.nus.edu | Windows 10 / Visual Studio 2022
Lee En En | 97609822 | leeenen@u.nus.edu | Windows 10 / Visual Studio 2022
Amanda Ang | 86992477 | amandaang@u.nus.edu | Windows 11 / Visual Studio 2019
Gabriel Goh Kheng Lin | 82833858 | e0411257@u.nus.edu | Windows 10 / Visual Studio 2019
