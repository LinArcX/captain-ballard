# captain-ballard ![Language](https://img.shields.io/github/languages/top/LinArcX/captain-ballard.svg?style=flat-square) ![RepoSize](https://img.shields.io/github/repo-size/LinArcX/captain-ballard.svg?style=flat-square) ![Latest Tag](https://img.shields.io/github/tag/LinArcX/captain-ballard.svg?colorB=green&style=flat-square)
A git-aware app that protects your git-based projects before it's too late :)

## Preview
!["gnulium"](./shots/1.0.0/status.png "captain-ballard")

## A long time ago..
I had many projects on my computer some of which were managed by git. After a couple of months, I accidentally lost all of my data and projects.
And this gave me a good motivation to start a project that monitors my projects and reports me the changes periodically.

## Features
Watches __.git__ directories on your computer and periodically reports the latest changes popping up a window titled: __status_window__.

You can adjust the interval as most convenient for you.

### [Void](https://github.com/void-linux/void-packages/pull/14916)
`sudo xbps-install captain-ballard`

### [Arch](https://aur.archlinux.org/packages/captain-ballard/)
`sudo trizen -S captain-ballard`

### Build From Source
2. Install these dependencies first:

|Dependencies||
|-----|:-----:|
|[cmake](https://www.archlinux.org/packages/extra/x86_64/cmake/)
|[pkg-config](https://www.archlinux.org/packages/core/x86_64/pkgconf/)
|[gtk+3-devel](https://www.archlinux.org/packages/extra/x86_64/gtk3/)
|[glib-devel](https://www.archlinux.org/packages/core/x86_64/glib2/)
|[sqlite-devel](https://www.archlinux.org/packages/core/x86_64/sqlite/)
|[libgit2-devel](https://www.archlinux.org/packages/extra/x86_64/libgit2/)
|[libappindicator-devel](https://www.archlinux.org/packages/community/x86_64/libappindicator-gtk3/)

3. clone the repo:

    `git clone https://github.com/LinArcX/captain-ballard/`

4. go to cloned directory and make the project with qmake build tools:

    `cd captain-ballard`

    `mkdir build; cd build/`

    `cmake -DCMAKE_BUILD_TYPE=Release ../`

    `make`

5. run the application:

    `../release/captain-ballard`


## Contributing
![Open PR](https://img.shields.io/github/issues-pr-raw/LinArcX/captain-ballard.svg?style=flat-square) ![Closed PR](https://img.shields.io/github/issues-pr-closed/LinArcX/captain-ballard.svg?style=flat-square)
1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D


## Bugs
![Open Issues](https://img.shields.io/github/issues-raw/LinArcX/captain-ballard.svg?style=flat-square) ![Issues](https://img.shields.io/github/issues-closed-raw/LinArcX/captain-ballard.svg?style=flat-square)

Bugs should be reported [here](https://github.com/LinArcX/captain-ballard/issues) on the Github issues page.


## Credits
E-Mail:linarcx@gmail.com

<h1 align="center">
	<img width="200" src="./util/icons/captain.svg" alt="captain-ballard">
	<br>
	<br>
    <p2>captain-ballard</p2>
</h1>

## License
![License](https://img.shields.io/github/license/LinArcX/Gnulium.svg?style=flat-square)
