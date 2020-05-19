## captain-ballard
A git-aware app that protects your git-based projects before it's too late :)

<h4 align="center">
    <img src="assets/appconf/captain-ballard.svg" align="center" width="100"/>
</h4>

<h4 align="center">
    <img src="https://img.shields.io/travis/LinArcX/captain-ballard"/>  <img src="https://img.shields.io/github/tag/LinArcX/captain-ballard.svg?colorB=green"/>  <img src="https://img.shields.io/github/repo-size/LinArcX/captain-ballard.svg"/>  <img src="https://img.shields.io/github/languages/top/LinArcX/captain-ballard.svg"/>
</h4>

<h1 align="center">
    <a href="https://gist.githubusercontent.com/LinArcX/c8e9bc4eec041116639293abb011a9b0/raw/4db84504fc1e97a8c897cceccbfa3e50cbe73e6c/captain-ballard-1.0.0-status.png"><img src="https://gist.githubusercontent.com/LinArcX/c8e9bc4eec041116639293abb011a9b0/raw/4db84504fc1e97a8c897cceccbfa3e50cbe73e6c/captain-ballard-1.0.0-status.png"></a>
    <br/>
    <h4 align="center">List of all projects that captain-ballard are watching. <a href="https://github.com/LinArcX/captain-ballard/issues/1">[more]</a></h4>
</h1>

# NO LONGER MAINTAINED. Looking for an alternaive? See: https://github.com/LinArcX/giti

## A long time ago..
I had many projects on my computer some of which were managed by git. After a couple of months, I accidentally lost all of my data and projects.
And this gave me a good motivation to start a project that monitors my projects and reports me the changes periodically.

## Features
Watches __.git__ directories on your computer and periodically reports the latest changes popping up a window titled: __status_window__.

You can adjust the interval as most convenient for you.


## Installation
### Void
`sudo xbps-install -S captain-ballard`

### Arch
- git version

`trizen -S captain-ballard-git`

- release version

`trizen -S captain-ballard`

### Build From Source
First Install these dependencies first:

#### Dependencies
build dependencies:
- cmake
- pkg-config

runtime dependencies:
- gtk+3
- glib
- sqlite
- libgit2
- libappindicator

Then clone and build the project:

```
git clone https://github.com/LinArcX/captain-ballard/
cd captain-ballard; mkdir build; cd build
cmake ..; make
```

And finally, run it:

`cd ..; build/captain-ballard`

## Donate
- Monero: `48VdRG9BNePEcrUr6Vx6sJeVz6EefGq5e2F5S9YV2eJtd5uAwjJ7Afn6YeVTWsw6XGS6mXueLywEea3fBPztUbre2Lhia7e`
<img src="assets/monero.png" align="center" />

## License
![License](https://img.shields.io/github/license/LinArcX/captain-ballard.svg)
