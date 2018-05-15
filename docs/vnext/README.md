# Napa.js vNext (0.4.x-canary)
vNext is a version with new worker implementation. In Napa.js vNext, napa worker loads and initializes its javascript VM in node.js style, which enables most of node.js APIs and addon support.

## Features in vNext
- Better Node.js API compatibility. Most of [Node APIs](https://nodejs.org/api/index.html) will be supported by default. See [this](./node-api-compatibility.md) for compatibility tracking.
- Native addon support. All native addon can be loaded in worker by default, unless it's implemented non-thread-safely. See [this](./node-native-addon-compatibility.md) for compatibility tracking.

See also: [Open Issues](link-to-issue:TBD), [Project vNext](link-to-project:TBD), [Napa.js versions](./napa-versions.md)

## Install
Napa.js vNext requires a patch to node to work. You can either use the pre-built binaries or build node by yourself.

*We are working on pushing those changes to node. [(tracking issue)](link-to-issue:TBD). Napa.js will work in an official node release in future.*

### Use prebuilt binaries:

**Install or upgrade to Node.js v10.1.0.**

Install the latest canary version:
```
npm install napajs@canary
```
This should fetch the binaries for both patched node and napajs.

### Build patched node.js and napa.js

Refer to node [Bootstrapping Guide](https://github.com/nodejs/node/blob/master/tools/bootstrap/README.md) and [Build Napa.js](https://github.com/Microsoft/napajs/wiki/build-napa.js) for prerequisites. 

- Get node source code and checkout v10.1.0
  ```
  git clone https://github.com/nodejs/node.git
  cd node
  git checkout v10.1.0
  ```
- Apply changes [@34ca9f3](https://github.com/nodejs/node/commit/34ca9f3b910127667614b138fc2a96ca24c8518c)
  ```
  git cherry-pick 34ca9f3b910127667614b138fc2a96ca24c8518c
  ```
- Build from source
  > Windows:
    ```
    vcbuild
    ```
  > Linux & macOS:
    ```
    ./configure
    make
    ```
- Set environment variable `NAPAJS_NODE_EXE` and `NAPAJS_NODE_LIB` (windows only)
  > Windows:
    ```
    set NAPAJS_NODE_EXE=<your-node-repo-folder>\Release\node.exe
    set NAPAJS_NODE_LIB=<your-node-repo-folder>\Release\node.lib
    ```
  > Linux & macOS:
    ```
    export NAPAJS_NODE_EXE=<your-node-repo-folder>/out/Release/node
    ```
- Build Napa.js
  - Clone the source code 
    - Enter command line
    - Enter the directory that napajs will be cloned in its sub-folder named 'napajs'
    - `git clone https://github.com/Microsoft/napajs.git -b v0.4.x-canary`
  - Run command `npm install --no-fetch` at the root directory of local repo.

## Use patched node:
Update environment variable `PATH` before using the patched node:

> Windows:
  ```
  set PATH=<your-napa-root-folder>\bin;%PATH%
  ```
> Linux & macOS:
  ```
  export PATH=<your-napa-root-folder>/bin:$PATH
  ```
`<your-napa-root-folder>` should be a full path, something like `D:\code\my-project\node_modules\napajs\bin` (Windows) or `/Users/name/code/my-project/node_modules/napajs/bin` (macOS)

*If you are using `nvm`, use the following command to deactivate npm:
```
nvm deactivate
```
