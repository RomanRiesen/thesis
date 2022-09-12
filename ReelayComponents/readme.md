# Introduction

These are (supposed to be) reusable components to enable quick integration of runtime verification for F' systems both for comprehensive on-the-ground testing as well as in-flight. Though they are certainly not ready for in-flight use and their reusability is still a bit lacking.

For an introduction to all the terms used above I'd like to refer to the thesis also contained in this repository.


# Usage

The usage is described in the thesis. In short one needs to copy the folder with the reelay components into the deployment and add two cmake lines, one for inclusion of the cudd librariy (used by the reelay monitors used by us) and one for including the components.
