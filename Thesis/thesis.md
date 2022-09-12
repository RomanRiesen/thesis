---
title: On Integrating Runtime Verification in F' Architectures
author: Roman Riesen
date: 8.8.2022
geometry: a4paper
bibliography: bib.bib
csl: transactions-on-parallel-computing.csl
link-citations: true
geometry: "left=3.5cm,right=3.5cm"
fontsize: 12


documentclass:
    - article
header-includes:
    - \usepackage{hyperref}
    - \usepackage{tikz}
    - \usetikzlibrary{shapes.multipart}
    - \usepackage[backend=biber,style=numeric, backref=true]{biblatex}
    - \bibliography{bib}
---

\begin{titlepage}  
  \begin{center}  
  
  \begin{figure}[t]  
  \vspace*{-2cm}        % to move header logo at the top 
  \hspace*{-1cm}
  \raggedright{\includegraphics[scale=0.3]{figures/UNI_Bern.png}}
  \vspace{0.6in}     
  \end{figure}

    \thispagestyle{empty}
    
    {\bfseries\Huge On Integrating Runtime Verification in F' Architectures \par
    \Large \vspace{0.1in} \par}

    \vspace{0.2in}
    {\large By \par}
    \vspace{0.2in}
    {\Large Roman Riesen \par}
    \vspace{0.4in} 
    \LARGE{\textbf{Bachelor Thesis} \\}
    
    \vspace{0.2in}
    {\Large Software Engineering Group \par}
    {\Large Department of Computer Science \par}
    {\Large Philosophisch-Naturwissenschaftliche Fakult\"{a}t \par}
    {\Large Universit\"{a}t Bern \par}
    \vfill
    {\Large September 2022 \par}
    \vspace{3cm}
    {\raggedright
    {\normalsize \hspace{4.8cm} \textbf{Supervision:} \par}
    {\normalsize \hspace{5.2cm} Dr. PD Christos Tsigkanos \par}
    {\normalsize \hspace{5.2cm} Prof. Dr. Timo Kehrer \par}
    }


  \end{center}

\end{titlepage}

\newpage

\pagenumbering{Roman}

\setlength\parindent{12pt}

\newcommand{\nxt}{\bigcirc}
\newcommand{\evtl}{\lozenge}
\newcommand{\hforth}{\square}
\newcommand{\until}{\emph{U}}
\newcommand{\globally}{\emph{G}}
\newcommand{\N}{\mathbb{N}}
\newcommand{\D}{\mathbb{D}}

\tikzset{
state/.style={
       rectangle split,
       rectangle split parts=4,
       rectangle split part fill={yellow!20,white!20,white!20,white!20},
       rectangle split part align={centered, left, left, left},
       %rounded corners,
       draw=black, very thick,
       minimum height=2.5em,
       text width=8cm,
       inner sep=2pt,
       }
}

\begin{center}{ \Large \textbf{Abstract}
}
\end{center}

Reliability and safety requirements are especially pronounced in the case of aerospace, robotic, and other critical systems. Space software, in particular, has historically enjoyed prime applications of formal methods. Such software is carefully designed, implemented, and verified against requirements before launch and deployment. Runtime verification is a flight-proven, lightweight method based on observing information from a system while in operation, and identifying if observed behaviors satisfy or violate certain requirements. It scales well in complex systems, and verdicts of monitored requirements may also be used for control decisions throughout the system operation. However, applications of runtime verification within space software have largely been residing at the implementation level. In this thesis, we investigate the development of runtime verification support at the architectural level. We specifically target JPL's F', a framework enabling rapid development and deployment of spaceflight and other embedded software applications. We follow a principled approach, with the overall goal to enable existing F' systems to integrate monitoring of requirements expressed as temporal logic statements in a reusable manner. We illustrate runtime verification over a rocket model, and outline design decisions as well as supporting implementation adopting Metric Temporal Logic for specification and Reelay monitors for verification.

\newpage

\begin{center}\Large \textbf{Acknowledgements}
\end{center}

I wish to thank Dr. Tsigkanos for his ever-present enthusiasm and support as well as Prof. Kehrer for all the accommodations one could hope for.
Further, I am grateful to my family for all their support.

\newpage

\tableofcontents
\newpage
\listoffigures

\newpage

\pagenumbering{arabic}

# Introduction

## Problem Setting

Growing demand for software-heavy systems of growing complexity and working in growing scopes leads to an increase of software practitioners requiring accessible ways of achieving dependability. The requirements of reliability and safety are especially pronounced in the case of aerospace systems, robotics, and other critical systems as software errors might prove devastating to the missions in question.

Formal verification is the proving of the absence of incorrect behavior. Often this is achieved by model checking, which is traditionally done by exhaustively proving or checking that all states of a program are permitted by a specification. Model checking of a whole system is often either infeasible or too costly.
Deductive formal verification approaches in which mathematical proofs are constructed to show the correctness of the program require expertise and are also very work intensive. And whilst more lightweight deductive proof techniques such as dependent typing have made big strides they are still far from production ready.

Testing may be relatively cheap but the guarantees provided are weak as the absence of insecure states cannot be proven and they rely heavily on an engineer's ability to notice potential edge-cases. Rare events might go ignored or unrecognized. Whilst fuzzy-testing does help in regards to rare-events testing it still lacks the hardness of the guarantees given by formal methods.

Runtime verification (RV) can offer a tradeoff between testing and formal verification. RV does not require a model. But it still allows for a very declarative way to define the required properties and check them upon a trace of system events occurring at system operation. Because not the entire state of the program is verified RV can also be used in bigger projects as the verification time scales super-linearly in model checking.

Advantages are further that hardware faults may be detected by the same properties that are used to verify software correctness and thus control sequences may be aborted before damage is done without much additional code. This holds too for the detection of potentially malicious commands or unforeseen emergent behavior.
The verdicts of the monitored properties can also be used in the normal control flow decisions as the temporal logic can be quite a bit more expressive than ad-hoc detection of event sequences.
Thus even if model-checking is possible one might still desire runtime verification.

[](and in combination with provably correct recovery code can often even replace the need for full-system verification, which allows for the correctness/safety of larger systems than model-checking)

Another important issue regarding the growth in software-heavy embedded systems is the ability for code reuse and maintainability. As the hardware can be limiting, commonly used abstractions of the wider software-engineering world are often avoided in favor of performance. This is where software architecture and supporting frameworks can be leveraged to great effect. Many tools try to use compile-time guarantees and constructs to simplify development and provide abstractions that allow the decoupling and encapsulation of components without incurring runtime costs. 
These two architectural goals enable teams to work more independently on different parts of the system, facilitate testing of the components themselves, and increase their reusability.

Creating abstractions to improve programmer productivity without strongly affecting runtime performance has been an ongoing concern and goal of software engineering since its beginning. 

F' is a framework and architecture that, as we will see, fulfills these requirements.

\newpage
## Framework Support for Runtime Verification

The falling costs for bringing payloads to low earth orbit have resulted in a market for lower-cost space systems, such as so-called cubesats [@villelaThousandthCubeSatStatistical2019]. This development did lead to hardware and software that allows smaller and less well-funded teams to develop space systems. The interest in these low-cost space systems is both scientific and industrial. 

In the following paragraphs we will introduce the tools used in the thesis and justify our choices.

One of these software systems is F', which is an architecture and framework developed by NASA's JPL for rapid development of flight software, but it can and has been used in general embedded systems as well. The architecture that F' encourages is one based on reusable components and connections thereof.

There are - as mentioned - already a great many uses of runtime verification in space systems.
Reelay is a header-only library for runtime verification, which allows for online monitoring of temporal logic properties.  It is fundamentally not dissimilar to a lot of other runtime verification frameworks that are commonly used by teams building critical systems.

Whilst there are fast and ready-to-use libraries providing RV based on temporal logic such as R2U2 [@moosbruggerR2U2MonitoringDiagnosis2017] there seemingly has not been any work on providing reusable components for runtime verification. We believe the that advantages of such would align strongly with the use cases of the F' framework and fit well with its architecture.

Since the goal of F' is enabling engineers to quickly produce software for small-to-medium sized embedded systems having an easily reusable component for runtime verification of temporal properties is desired. There is work underway to easily integrate the SPIN model checker [@SpinFormalVerification]. But as we reasoned in the previous chapter this would not fully remove the need or desire to support runtime verification. This thesis aims to produce such an RV component.



## Contribution

The combination of tools and created software artifacts presented in this thesis have the following purposes and goals:

- A principled approach to enable existing F' systems to easily integrate monitoring of temporal logic statements
- Support engineers to use a declarative style to specify Instrumentation to monitor and the recognize of sequences of states
- Using F' enables both ease of deployment and moving verification monitors to a different physical chip from information creating/gathering chips, if performance concerns arise.

In the taxonomy of software engineering research laid out by [@shawWhatMakesGood] this thesis is part of the development and extension phase for runtime verification in F'.

[related_work]:


## !!! Related Work

Reelay, which is adopted in this thesis as the underlying verification engine is similar to R2U2 [@schumannRuntimeAnalysisR2U22016]. Thanks to its inclusion of bayesian networks TL properties can be more involved whilst still allowing to conclude the origin of the error. 

If one were to use MagicDraw [@MagicDrawCATIADassault] with the SysML plug-in one can generate F' topologies in a diagramming software. Therefore this approach might superficially be a similar experience to using Simulink [@SimulinkSimulationModelBased]. Both approaches are closed-source in nature and might thus not be applicable or desirable under certain circumstances. And using Simulink to generate F' Components might still require significant engineering effort, but might be an avenue worth exploring in future research. One downside of both these graphical approaches is the lack of suitability to commonly used version control systems of the underlying files.

There are many more tools not explicitly listed here but which might be worth one's consideration for FSW development [@caldwellSmallSpacecraftAvionics2021]. Many of the other solutions seem to lack the popularity and momentum of F'. And many of the tools developed by NASA can be seen as predecessors to F' but lack the cohesion and tools it provides.

In regards to implementing Runtime Verification in flight systems with small-to-medium scope, there is some work done by students [@hertzIntegratingRuntimeVerification2021] and older work by bigger teams [@goldbergRuntimeVerificationAutonomous2005].







\newpage
# Background

## Motivating Example

As an example application, we will consider a simple rocket model of which we will build an F' component, simulating possible sensor outputs. We then consider a few properties that one might verify in a real system. The numbers and details chosen are in line with typical student competition rockets such as in [@hertzIntegratingRuntimeVerification2021].

 The simulation is based on the classic Tsiolkovsky rocket equation[@ManEquationRocket] with random error terms applied - simulating measurement errors or differences from the theoretical ideal of the system - so that the system may violate the defined properties. Pressure effects are ignored.

 The rocket has an accelerometer measuring acceleration ($a_m$), burn rate ($b$), fuel weight ($m_f$), exhaust velocity ($v_e$), and weight of the empty rocket ($m_0$). We also assume there is a ground station system that can measure the velocity of the rocket (via the doppler effect). This velocity we will call ($v_d$).
 The status of the rocket is one of "prelaunch", "firstStage", "secondStage", "descent", "landed".

 We also let $E_a, E_d, \epsilon_b$ be independently normally distributed random variables with mean 0 and variance $\sigma_a, \sigma_d, \sigma_b$, respectively. The first two random variables represent the variance of measurement of the accelerometer, of the ground-to-rocket doppler-effect measurement. The last variable is the variance of the burn rate of the engine.

 The Tsiolkovsky rocket equation formulates the force on the rocket at a point in time ($t$) by the change of mass (fuel) multiplied by the exhaust velocity multiplied by the current rocket mass:

 $$F = ma = (m_0 - b t) (v_e b)$$

 From this we derive a few more variables and introduce the simulated errors and inaccuracies:

 - $a_r$, the actual rocket acceleration given by $\frac{v_e (b + \epsilon_b)}{m_0 - bt}$, where t is time. The maximal burn time being $\frac{m_f}{b}$
 - $a_m = a_r + E_a$
 - $v_I$, the velocity calculated by integrating the accelerometer data

 A few of the properties we might want to specify are:

 In the scenario presented, a system designer may be interested in checking if certain requirements hold at runtime, when the rocket system is in operation. For our purposes we will consider the following properties:

 - The status must start at "prelaunch" and after that increase in order
 - 10 seconds after firstStage the burnrate must be more than $20 kg/s$
 - The difference between $v_I$ and $v_d$ should always be less than $1 m/s^2$ when the status is either `firstStage` or `secondStage`

\begin{figure}[h]
\centering
\includegraphics[width=0.8\textwidth]{./figures/rocket_height_graph.png}
\caption{The height of the rocket over time as plotted by the F' ground system (introduced later)}
\label{fig:rocket_height_graph}
\end{figure}


## A walk through F´

F' (F Prime) is an architecture, and framework in support of that architecture, which is being developed by NASA. Its stated goal is to enable component-driven rapid development and deployment of spaceflight and other embedded software applications [@MoreCompleteIntroduction]. F' takes a model-based approach to creating software; letting the developer express the high-level structure in a model which consists of a data structure and formal specification[@jrFPPModelingLanguage]. F' makes use of typed interfaces to provide compile-time guarantees and has built-in unit and integration testing facilities.

There exist a variety of flight-tested components already [@Features]. F' also offers operating-systems-like functionality and thus can be used cross-platform on both bare-metal and desktop systems which eases development further.

Successful real-world deployments of it exist, such as the Ingenuity mars helicopter [@https://www.jpl.nasa.govMeetOpenSourceSoftware]. 

What follows is a short introduction to the F' concepts necessary to understand the design and decisions in the rest of the thesis. For a more thorough introduction, there is an excellent overview by Bocchino et al [@bocchinoPrimeOpenSourceFrameworka]. There is a separate overview for the FPP modeling language (which was introduced in F' 3.0.0) by them as well [@jrFPPModelingLanguage].

### Workflow

The main building blocks in F' are *components*, which have *ports*, which in turn are connected to each other by *connections* defined in *topology* definitions. The components are analogous to C++ classes; ports to their methods. Topologies do not have a one-to-one mapping to C++, they are both a way to structure bigger projects and a way to represent interaction.

The workflow of using F' is as follows:

1. Design a topology from the Requirements
2. Decide what existing components can be used and which have to be created
3. Specify the components the system requires (either in XML, FPP, or MagicDraw)
4. Specify how and what the components exchange via ports
5. From these specifications, C++ files are generated that already contain the necessary infrastructure and organization to quickly implement the business logic and receive commands.
6. Implement the C++ components
7. Generate and write a unit test for each component
8. Create a topology definition that instantiates components and connects these instances
9. Test the topology


### Operating System Abstractions

By providing operating-systems-like functionality, such as thread management, synchronization primitives, and input and output, F' deployments can be run in a multitude of operating systems and bare-metal environments, easing development. However, the support of thread virtualization is considered experimental and should not be used in production [@BaremetalMultiCoreSystems].

### Components

Components represent a physical or virtual part of the system. Such as sensors or logging systems. They provide encapsulation and thus should not communicate with other components, except by their connected *ports*.

The use of components is encouraged by the availability of tools for their creation, reuse, and testing. With the `fprime-util` suite of python tools, it is easy to create the C++ classes and corresponding tests from an FPP specification. This suite of tools also streamlines the building and running of components and deployments respectively.

Components are either `Active`, `Passive`, or `Queued`. This designation determines what synchronicity of invocations the component supports and how those are handled. `Active` and `Queued` components have buffers for their `input` ports. The former owns a thread on which invocations to its ports are processed. The latter needs the work to be done by an `Active` component. `Passive` components are exactly C++ classes and provide no queue or thread management. They are useful for data transformation between `Active` and `Queue` components. More on why we need this differentiation will be discussed in the next section.

As mentioned there are default components already available in F', such as the `ActiveLogger` which takes telemetry and events and logs them or converts them to telemetry, or the often used `RateGroupDriver`. This component can be thought of as a clock. its output is usually connected to an `ActiveRateGroup` component whose output in turn is used to schedule the code execution of components at a fixed rate. However, if the executed code takes longer than the rate, the next invocation will be delayed and a warning of high priority will be emitted.
From component specifications, one can derive boilerplate for unit tests. This is done by creating a new CMake executable to which one adds the paths to the `.fpp` and `.cpp` component definition, as well as a `main` C++ file to drive the test. The derivation of that file is done by calling the `fprime-utils impl --ut` command that produces a test main function as well as the boilerplate for the component instantiation which is required by a test. Thus a lot of potential work is saved.

To ease testing and verification further there is an exploration [@jrFPPModelingLanguage] of directly incorporating the spin [@SpinFormalVerification] model checker as well as a scenario-based testing specification system.

### Ports

The interface of components is defined by their ports. Those are in turn defined by their type and their kind. The type of a port be zero or more F' or primitive types and can specify argument and return types. The kind of a port is either output or `input`.

Ports are also one of `synchronous`/`asynchronous`/`guarded`. These, in combination with the type of the component, determine how the work done by components on `input` port invocations is handled.
`Passive` components may only have `synchronous` or `guarded` input ports as they lack a queue to store and process the invocation parameters. That would be required to later execute the invocation asynchronously. 

`Active` and `Queued` component types must have at least either one `async` input port or `async` command.

Special types of ports represent the receiving of commands (from the ground station) as well as events and telemetry. Also, there are serial ports that can be connected with ports of any type.

*Serial Ports* can be connected to any type of port. The cost of this flexibility is the loss of compile-time safety of the interface type checking and the reconstruction of data from such a port is solely the programmer's responsibility. A side-effect of the architectural decision to only allow communication via ports is that it was relatively easy to create patches to ignore broken sensor input and replace it with something reasonable in the mars helicopter mission [@NASAMarsHelicopter].

*Event Ports* are used to communicate notifications or warnings and they can contain data. For example: 

```C++
    event OVERFLOW_DETECTED(var1: U32, var2: U32) \
        severity activity high \
        format "Overflow detected: {}, {}!"
```

would declare a warning event of high severity and two values, and in the ground station log would be displayed as in \ref{fig:overflow}.

\begin{figure}[h]
\centering
\includegraphics[width=0.8\textwidth]{./figures/multiplication_overflow.png}
\caption{Multiplication Error Event}
\label{fig:overflow}
\end{figure}

### Ground Station

F' provides a ground data system (GDS) that allows the dispatching of commands and displaying of telemetry received. GDS supports creating a dashboard - a custom GUI layout - from predefined GUI components, which are intended to give a quick overview of the data available to the GDS from a running embedded system [@GDSDashboardComponent].
The GDS is implemented using web technologies and opens a web interface on the ground station computer thus it can run on the same computer as the F' deployment or on a separate one.

!!! add image

### FPP

FPP is a modeling language that compiles to XML and C++ and has a more succinct syntax compared to writing the component and topology specifications by hand in either XML or C++[@HomeFprimecommunityFpp]. Thus FPP furthers the main goals of rapid development of the F' ecosystem. Its parser also has better error reporting than the XML parser.
Since version `3.0.0` F' comes with the FPP tools by default.

There is a plugin to support development inside Visual Studio Code [@martindaleFPPTools2022], though it currently does not much other than code-highlighting [^1].

[^1]: But it does it well and even sets the parts of `.fpp` files that contain C++ correctly in C++

The syntax is c-like and sparse but expressive. For example, a component could be defined as follows (in e.g. `f32adder.fpp`):

    @ a component for adding numbers
    passive component F32Adder { 
        constant port_nr = 2
        @ Input
        sync input port valueIn: [port_nr] F32
        @ Output
        output port valueOut: F32
        @ Telemetry
        telemetry port tlm

        # Telemetry channel definition
        telemetry VALUE : F32
    }

The lines starting with `@` will be translated into comments in the generate C++ file whilst the lines starting with `#` are proper FPP comments and will be stripped.

The component in turn would be instantiated and named `f32adder` like this (usually a separate fpp file named `instances.fpp` is used):

    instance f32adder: F32Adder base id 0xE00

the `base id` is used as an offset for the telemetry channels.

The instance could then be connected to other instances in a topology definition file (e.g. `topology.fpp`):

```C++	
    connections f32example {
        f32generator1.valueOut -> f32adder.valueIn[0]
        f32generator2.valueOut -> f32adder.valueIn[1]
        f32adder.valueOut -> f32consumer
    }
```

where `f32generator1`, `f32generator2`, and `f32consumer` are assumed to be instantiated somewhere else.

The final steps of the workflow are adding the `.fpp` files of the components and other type or port definitions to the *cmake* build system. Afterward one can then use the fprime tools to generate the boilerplate code for the topology and the component. In the generated `.cpp` and `.hpp` files, one can then define what happens exactly when another component *invokes* (or calls) a port (by implementing the corresponding method).


### SysML and MagicDraw

This is a combination of tools that allows for the graphical drawing of F' topologies. It was unavailable to us during the writing of this thesis thus we cannot relate how well the Reelay component integrates with that environment - or the environment in general.
But there seems to be criticism to this combination of tools by experienced F' practitioners [@jrFPPModelingLanguage], highlighting the non-trivial increase in complexity and the lack of ability to generate the ground system dictionary, which contains the data specifying what commands, parameters and telemetry channels are available (this is done automatically by the `fpp-compile` tool thus detail it is not mentioned otherwise).
For the above reasons we decided to not elobarate on the impact of our components onto this toolchain.


## Temporal Logic

Whilst the history of combining temporal properties with what we today call propositional logic and the resulting questions of valuations of sentences have their roots in ancient Greece (namely Aristotle and Diodorus Cronus in the third- to second-century B.C.E., e.g. the *master argument* of the latter [@ohrstromFutureContingents2020]), we will focus on the more formal work developed in the middle of the 20th (C.E) century.

We call any logic that can express temporal behavior *temporal logic*. In the narrow sense, these have their roots in modern analytical philosophy [@gorankoTemporalLogic2022] and later found many applications in formal verification and as mentioned in runtime verification. Initially, these were *branching time* temporal logics, which view time as a tree structure where the future is one path of many on that tree and the past is known and is thus a linear graph.
As algorithms for determining satisfiability of linear complexity in formula size as well as model size exist for CTL they have become prevalent in model checking [@katoenComplexityConsiderations]. But CTL suffers from a lack of expressivity (later remedied by CTL*)[@gorankoTemporalLogic2022] such that formulas in CTL can be exponentially bigger than LTL.

Much later on TLA^+ was developed by Leslie Lamport, which builds on top of first order logic and set theory, thus being much more expressive than the other logics treated here. This system is widely used in industry [@aitameurAbstractStateMachines2014].

\label{LTL}

### LTL

Linear Temporal Logic is an extension of classic propositional logic by operators representing a notion of time. 
This is achieved by having a sequence of assignments for the propositional variables, $W_i \; (\forall i \in \D)$ where $\D$ is the time domain, which has a total order and can either be dense[^2] or discrete. As the name implies this kind of logic considers the future to be fixed in the sense of there existing only one branch that can be taken, with the past being a line (the path taken so far)[@gorankoTemporalLogic2022].

[^2]: In the usual mathematical sense of *dense order*[](It also must be Dedekind-complete (every non-empty subset must have a supremum) as otherwise, intervals might not be well-behaved.)

Some common operators are defined as follows for the variable $p$ at time point $j$ on a discrete time domain:

+--------------------+-------------------+-----------------------------------------------------------------+
| $\nxt p_j$         |    Next           |  $p_{j+1}$ must be true  (only exists in discrete time)         |
+--------------------+-------------------+-----------------------------------------------------------------+
| $\evtl p_j$        |    Eventually     |  $\exists i \geq j:$ such that $p_i$ holds                      |
+--------------------+-------------------+-----------------------------------------------------------------+
| $\hforth p_j$      |    Henceforth     |  $\exists i \geq j:$ such that $\forall k \geq i: p_k$ holds    |
+--------------------+-------------------+-----------------------------------------------------------------+
| $p_j \until p_k$   |    Until          |if $\exists k: p_k$ then $\forall i \leq k: p_i$ else $\bot$     |
+--------------------+-------------------+-----------------------------------------------------------------+
| $\globally p_j$    |   Globally        |  $\forall i \in \D p_j$ holds                                   |
+--------------------+-------------------+-----------------------------------------------------------------+


### MTL

Metric Temporal Logic is similar to LTL with an addition of a metric for time, enabling the expression of durations. The operators thus take some form of duration specification, usually in the form of an interval, we write `op[t0:t1]` with $t0, t1 \in \D$ for the metric version of LTL operator `op`, which is only checked on the interval from time point `t0` to `t1`. Not all LTL operators have a reasonable metric version, in the above table $\nxt$ and $\globally$ can not have such time restrictions as they already imply their interval (e.g. $\nxt p_j$ from $t0=j+1$ to $t1=j+1$ and $\globally p_j$ from $t0=\inf \D$ to $t1=\sup \D$).

### Runtime Verification

The goal of runtime verification is to specify the desired behavior of a system and then check the adherence to that specification at runtime (online) or afterward (offline). We focus mainly on the former case. This is done by generating a *monitor* from the specifications which is then connected to the running system in such a way that it has access to all the data required to verify the specification. The monitor is run on a *trace* of a system. A trace is for, the purpose of our thesis, a totally ordered set of sets of variables and their values. From the specification, one derives *properties*, which describe a set of traces. A property is said to hold for a trace if the current trace is in the set defined by that property. The monitor checks if a given specification is satisfied on a trace. [@bartocciIntroductionRuntimeVerification2018]

- *Monitorability* is the trait of a property to be determined from the underlying system. A trivial example of a property that is not monitorable in our rocket example is "the rocket's angle may not exceed one degree for the first 90 seconds of the flight" because the data required is not part of the system.

- *Polarity* of a specification is whether it captures a state that is allowed or not allowed. For example, " "The difference between the integrated speed and the speed measured by doppler effect exceeds 1m/s" has negative polarity as it specifies a state that is not allowed. We will assume positive polarity in this thesis.


### !! Reelay

Reelay is a library that provides runtime monitors for past metric temporal logic (pmtl) [@ulusOnlineMonitoringMetric2019].
It enables the efficient monitoring of dense time metric logic by using a non-pointy (based on durations) definition.
One of the reasons for the achieved performance is the omission of future-time operators as they were introduced in \href{#LTL}{LTL}. Thus only statements about the past are possible. 

The Reelay monitor can either be discrete or dense. In the discrete case the total order mentioned previously is the only notion of time available. But in the dense case a `time` variable is expected to be part of the trace. The value of that variable may correspond to physical units such as milliseconds (the unit we chose for our implementation).

 The pre-defined temporal operators are:

+---------------------------+--------------------+-----------------------------------------------------------------------+
| `pre {A}`                 | `Y {A}`            | in discrete time: `{A}` was true at now-1, not available in dense time|
+---------------------------+--------------------+-----------------------------------------------------------------------+
| `once {A}`                | `P {A}`            |   Property `{A}` was true in at least one point in time (t < now)     |
+---------------------------+--------------------+-----------------------------------------------------------------------+
| `once[t0:t1] {A}`         | `P[t0:t1] {A}`     |   Property `{A}` at least once true in the specified time             |
+---------------------------+--------------------+-----------------------------------------------------------------------+
| `historically {A}`        | `H {A}`            | {A} true at all times in the past                                     |
+---------------------------+--------------------+-----------------------------------------------------------------------+
| `historically[t0:t1] {A}` | `H[t0:t1] {A}`     | {A} true at all points between t0 and t1                              |
+---------------------------+--------------------+-----------------------------------------------------------------------+
| `{A} since {B}`           | `{A} S {B}`        | true if {A} always true since {B}                                     |
+---------------------------+--------------------+-----------------------------------------------------------------------+
| `{A} since[t0:t1] {B}`    | `{A} S[t0:t1] {B}` | true if {A} always true since {B} was true between t0 and t1          |
+---------------------------+--------------------+-----------------------------------------------------------------------+

Where `t0`, `t1` are time bounds that can be omitted to get unbounded expressions.

`{A}` and `{B}` are atoms. 

The monitoring is implemented using sequential networks as opposed to the more commonly used approaches of rewriting rules or büchi-automata.

!!! give quick overview of bdd and the transformation and the issues with future temporal logic and pointy mtl



### RYE-format

RYE is the format used by Reelay for the specification of the properties to be verified [@RyeFormatReelay].
Atoms evaluate to either `true` or `false` but they can be constructed more expressively than in propositional logic.
The types of atoms allowed in the Rye are boolean literals, numerical comparisons, and string equality. The operations of propositional logic `not`, `and`, `or`, and `->` behave as usual.

A few properties of our rocket example expressed in RYE are:

 - The status must start at "prelaunch" and after that increase in order

```
{{{status = "firstStage"}    since not {status = "prelaunch"}}
or {{status = "secondStage"} since not {status = "firstStage"}}
or {{status = "descent"}     since not {stage = "secondStage"}}
or {{status = "landed"}      since not {stage = "descent"}}}
```

 - 10 seconds after ignition the burnrate must be more than $20 kg/s$

```
 {burnrate > 10 since[10:0] {status = "firstStage"}}
```

 - The difference between $v_I$ and $v_d$ should always be less than $1 m/s^2$ during `firstStage` and `secondStage`. Because Reelay formulas do not support arithmetic operations we have to provide the absolute difference manually, we'll call it `speedDelta`, which we can then use:

```
{{stage = "firstStage"} or {stage = "secondStage"}} -> {speedDelta < 1}
    or {stage="secondStage"}
```


For the verification to happen we of course need some data. Reelay has chosen the ubiquitous JSON-format for that purpose. The properties above could be evaluated upon a trace - a sequence of events occuring at runtime. in practice, and recalling our rocket example, a JSON sequence of events would have elements of the following form:

```Json
{
   "time": 300, "burnrate": 11.582, "speedI": 100.394,
   "speedD": 100.167, "speedDelta": 0.227, "status": "firstStage"
}`
```

Note that if the monitor is chosen to be discrete-time, the time field is not needed and the temporal intervals correspond to the number of events passed.

Related to events are two features of Reelay [@TemporalBehaviorsReelay]:

- *Condensing* refers to the ability of reelay to reuse events if only the time changes. This saves memory costs and decreases the number of events that need to be looked at when verifying a property. 
- *Persistence* refers to the fact that if an event is missing some values compared to a previous event the values of the earlier event are reused.

### C++

Because space systems that are targeted by F' generally strive for using as few resources as possible there are a few concessions to be made in the use of C++ features. As both RAM and ROM that are radiation-hardened cost magnitudes more than their terrestrial counterparts, features that increase the binary size (such as templates) are discouraged. Further, the use of the F' functions over `std`-library equivalents is encouraged for efficiency and bare-metal compatibility.

In the implementation, we tried to stick to the style and limited feature set suggested by [@CodeStyleGuidelines]. Though there were quite a few concessions made in the name of quicker development time and ergonomics, which due to Reelay not being written with these strict limitations in mind should not be too impactful a decision on either runtime performance or compile time.


\newpage
# Runtime Verification for F'

In a top-down approach, we will now present how the Reelay monitors were mapped to the concepts of F' and explain some of the design decisions and alternatives.
For ease of understanding, we will assume there to be a component `PhysicalSensor` representing some sort of data generator, the output of which we wish to verify with Runtime Verification. Following our example this component will interchangeably be called `RocketSimulator` in diagrams and later sections

## Monitor Adapter Pattern

One of the fundamental design challenges with using an off-the-shelve RV-library, like Reelay, is that to verify anything we first need to create inputs in a format that the library requires. In the case of Reelay that is - as mentioned - the JSON-format. As overhead is a major design concern of F' the more natural expression of such data therein would be binary streams, which are used liberally by component connections (the F' `serial` ports) and connections to external physical systems (using standard protocols like GPIO, UART). Thus there needs to be some sort of translation from the data-streams of sensors or F' provided components to the JSON-format required by Reelay. These generated events should then be validated at a fixed, and relatively high rate for the RV output to be useful.

One of the goals of the provided components is easy integration with existing components. Thereto we split the creation of events and the monitoring thereof into two components, called them `ReelaySensor` and `ReelayMonitor`. 
`ReelaySensor` creates the `ReelayEvent`s from data gathered from `PhysicalSensor`. These events are then sent to the `ReelayMonitor` which is the actual verifier and has an output port for the verdicts. This decouples the data transformation from the data verification, which in turn makes it possible to have different implementations of `ReelaySensor` whilst fully reusing the `ReelayMonitor`. The `ReelaySensor` is therefore similar to the *Adapter* in an adapter pattern [@gammaDesignPatternsElements1994].
The `ReelaySensor` components can also have ports to forward the incoming data to other components. This is useful if the `ReelaySensor` is used as part of a larger, existing system and one wishes to create Reelay events to send to a monitor, but have the data generated still be available as if connected to the original destination. 
One may use one or more instances of each component in one topology and use the output of one instance of the `ReelaySensor` as the input of another instance. This allows for the mixing of multiple approaches to the sensor components as later illustrated.


## ReelaySensor

Before diving into the details of both `ReelaySensor` and `ReelayMonitor` we will first give a quick justification of the diagram layout we have chosen to represent components. It is similar to the ubiquitous UML class diagram. But as we have seen previously F' components don't necessarily map one-to-one onto classes of object-oriented programming. Thus our diagrams have three categories of members instead of the typical two. The first one contains member variables and does specify their visibility: using `+` for public and `-` for private. The second category contains ports and subcategories noted in bold, such as `input` and `output`. The third, and last category contains the commands of the component. We note that the diagrams have neither an exact correspondence to the FPP definitions nor the C++ implementations and are not exhaustive. They aim to serve as illustration aids and are intended to be interpreted informally. Thus we also omit some types as they can get quite verbose and are not relevant to the understanding of the design, or simplify port types by giving the underlying type instead of the full port type, e.g. if a port type is `ReelayEventPort (event : ReelayEvent)` we will write `ReelayEvent` as a type.


\begin{figure}[h]
    \centering
    \begin{tikzpicture}
    \node [state] {
        \nodepart[align=center]{one} {\bf ReelayStaticSensor}
        \nodepart{two} {}\\
        \nodepart{three} {\bf Output Ports:}\\
        reelayEventOut \\
        eventOut \\
        rocketStatePort() -> RocketStatePort \\
        {\bf Input Ports:}\\
        schedIn \\
        {\bf Telemetry Ports:} \\
        reelayEvent \\
        \nodepart{four} {\bf Commands:\\}
        };
    \end{tikzpicture}
    \caption{ReelaySensor Component Diagram}
    \label{fig:reelayStaticSensor}
\end{figure}


The `ReelaySensor` is most naturally represented by a `queued` component, which emits events on a schedule set by a `rateGroupDriver`. These events contain the most up-to-date data of all the actual sensors for which this `ReelaySensor` instance is responsible. But as both `queued` and `active` components add quite significant overhead to the complexity of the system, their use is discouraged on bare-metal. We will use a `passive` component to maximize the usefulness of our provided components.

In regards to the `ReelaySensor` there are two approaches discussed and one implemented in the finalized topology example:

1. Having the input ports be of type `serial` and having a port that then specifies the data types expected on that port, we call this `ReelayGenericSensor`.
2. Having the input ports be statically typed, we call this one `ReelayStaticSensor`.

Version (1) has the advantage of being more reusable at the cost of being type-safe. It is less safe because a discrepancy between the topology definition and the runtime setting of the port types can lead to garbage data being sent to the `ReelayMonitor`. Also one has to ignore the warnings about ports not being connected if there are fewer input ports than the specified number as there is no documented way to parametrically specify the number of ports of a component instantiation in FPP (or XML). This approach will be discussed later on in \ref{genericSensor}.
The main issue with the `ReelayStaticSensor` is that one has to create a new F' component for every kind of `PhysicalSensor` one wants to use because the connections need matching port types on both ends. Therefore this version requires a new type of sensor for each kind of input one wishes to monitor, but it offers compile-time safety.

There is also the question of which component has the output port and which one has the input port. If the `ReelaySensor` has the input port we call it 'push based' as the component representing the physical sensor will have to invoke the port for data to travel to the `ReelaySensor`. On the other hand, if the port on the `ReelaySensor` is an output port that has a return value, declared in FPP along the lines of `output port get_data() -> SensorData`, then we call it pull-based as the `ReelaySensor` invokes the port to get the data. This differentiation is important because it determines which `schedIn` port determines the frequency of serializing the data; if the physical sensor has many more changes than we wish to forward to the monitor we can use the pull-based approach, however, if we wish to have every single change visible at the monitor we can use the push-based approach. Regarding these two design decisions we have the following two-by-two matrix:

\begin{center}
\begin{tabular}{c|c|l}
 Pull Based  & Push Based  &                  \\
 \hline
     (2)     &             & Typed            \\
 \hline
             &     (1)     & 'Runtime Typed'  \\
 \hline
\end{tabular}
\end{center}

There is a third, somewhat trivial approach - generating the `ReelayEvents` directly on the `PhysicalSensor` component, but as this is entirely deployment specific, we will not show it here. This approach is fine if the project intends to use the provided Reelay components from the beginning but it could be quite cumbersome to retrofit the corresponding ports into existing components. It also violates the single responsibility principle to an extent as the `PhysicalSensor` already has the purpose of providing data and transforming that data could be seen as orthogonal to that.


## ReelayMonitor

\begin{figure}[h]
    \centering
    \begin{tikzpicture}
    \node [state]
    {
        \nodepart[align=center]{one} {\bf ReelayMonitor}
        \nodepart{two} - properties\\
        - monitor\_options
        - monitors
        \nodepart{three} {\bf Input Ports:} \\
            eventIn : [5] string\\
            newProperty : ReelayProperty \\
            removeProperty :  \\
            time get timeGet \\
            {\bf Output Ports:} \\
            verdictOut \\
        \nodepart{four} {\bf Events:} \\
            NO\_PROPERTY\_REGISTERED() \\
            INVALID\_PROPERTY\_STRING (property) \\
            INVALID\_EVENT\_STRING (reelay\_event) \\
            NEW\_PROPERTY\_REGISTERED() \\
            NEGATIVE\_VERDICT (property) \\
            ALL\_PROPERTIES\_POSITIVE()
    };
    \end{tikzpicture}
    \caption{ReelayMonitor Component Diagram}
    \label{fig:reelayMonitor}
\end{figure}


This component contains the monitors provided by the Reelay library and manages the verification of properties. The verdict of which is then sent onwards to consumers, such as telemetry, components that are part of the control system, or by way of a `ReelaySensor` to other `ReelayMonitor` instances. 

The `ReelayMonitor` C++ implementation has a method `add_property` which registers the formulas to be verified. This can be used at runtime or at instantiation time like this:


```C++
    instance reelayMonitor: Ref.ReelayMonitor base id 0xE000\
        {
        phase Fpp.ToCpp.Phases.configComponents """
            reelayMonitor.add_property("{{status: 'firstStage'} \
                since not {status: 'Prelaunch'}}", 0);
            """
        }
```

The `add_property` method takes a string containing a Reelay formula and an id which can be used to delete the corresponding property (the management of the `id`s is up to the user). The method takes the property provided as a string and constructs a dense-time Reelay monitor from it.
The Reelay monitors used in our implementation are densely timed because the rate of verification is given from outside the component and might change at runtime. Therefore events need to have a `time` variable. If that is missing from the JSON event to the `ReelayMonitor` that component will add one in the unit of milliseconds as given by the special time port `timeGet`.


The monitor does the following on an `eventIn` port invocation from another component:

1. check if the event contains valid JSON, if not a high-priority event `INVALID_EVENT_STRING` is sent out
2. if the JSON does not contain a `time` field, one is added based on the `getTime` port which corresponds to the Unix timestamp. This time is in milliseconds.
3. check all properties individually if they are satisfied by the current values. If this is not the case a high-priority `NEGATIVE_VERDICT` event is sent out.
4. if all properties are satisfied a low-priority `ALL_POSITIVE` event is sent out.

The reason we have 5 `eventIn` ports is due to limitations concerning the F' string capacity discussed later in \ref{limitations}.


## Design Rationale

To communicate with a component one can either send it a command or invoke one of its ports. The first approach is mostly used for manually giving inputs to the system or high-level scripting by use of command sequences. The second way is used for communication inside the system. As we are designing components for reuse the balance to strike in regards to how much of a component is controlled by commands as opposed to input ports is non-obvious. The downside of having too many commands in pre-made components is making the command selection in the ground station cluttered and as the components are kept generic the command names might not be very useful to the issuer of commands, which may result in user error with possibly dangerous results. End-users can create components receiving commands and issuing the corresponding message to the provided components over ports easily, the main downside being more components and a more complex topology to maintain. In our view the downsides of generically named commands that may be confused by users outweigh the introduction of a slight increase in complexity, thus our provided components only use ports as inputs.

A similar issue exists in regards to communicating monitor verdicts from the `ReelayMonitor` to either other components or the GDS. One can have a telemetry port that sends the events directly to the ground station or an output port which communicates with other components. Relying only on the former approach has the downside that if such an verdict output port were not connected to a component communicating the violation of a property to the necessary places such errors might go entirely unnoticed during testing or simulation. This might prove catastrophic, later on.
The downside of the former approach is that a lot of potentially unnecessary communication is created, resulting in wasted energy and a more busy ground station log. We used a hybrid approach of having both ports available. To avoid cluttering the logs the logging of positive verdicts can be turned off, but to avoid an error detected by RV to go unnoticed the logging of a negative verdict is always logged with high priority. In this we assume that the properties of the system are expressed positively; that is if the result is `true` the system is assumed to work as intended in regards to the property covered by RV.

The `ReelayMonitor` does throw an exception if the registered formula is invalid but only if it was added at instantiation. If the formula was added at runtime the `ReelayMonitor` will simply ignore it and log a high-priority error. This is done to avoid the system crashing, but to make it obvious as close to compile-time as possible that one of the properties defined is invalid.

We use the dense-time behavior and do not provide an easy way to configure the options of the Reelay monitor used. One of the reasons for that decision is that verification behavior might change in unexpected ways when changing to discrete-time behavior and time losing any physical meaning. Therefore we chose to hide the monitor setting details.

## Limitations

\label{limitations}

As F' is intended to be usable on memory-limited hardware it generally avoids allocations at runtime thus the type `Fw::String` has a statically allocated buffer with a default capacity of 256 bytes (which can be adjusted by changing the macro declaration `FW_FIXED_LENGTH_STRING`). A result of this limitation is that one has to take care that the total size of one JSON event (sent from `ReelaySensor` to `ReelayMonitor`) fits into that size.
The solution would have been to implement an abstract FPP-type `JSON` to pass the data between the sensor and monitor. F' requires anything sent over ports to inherit from the `Fw::Serializable` abstract class. This in turn means that the serialized maximal size of the serialized JSON object would have to be known at compile time therefore simply increasing the limit (and decoupling it from the `Fw::String`'s capacity), not removing it entirely. But total overhead would be reduced by this approach as the JSON object would not have to be serialized and deserialized if not sent over special ports, therefore also enabling bigger properties to be sent.
On the other hand, because Reelay monitors themselves use the nlohman JSON library [lohmannJSONModern2022], which was not developed with the same restraint of runtime allocations in mind as F', the monitors aren't suited to low-memory systems.


## !!! Towards A Generic Sensor

\label{genericSensor}

Thus, as we have mentioned earlier, having a Generic Sensor would greatly ease the integration of our components into existing F' deployments. 
The analogous issue of having a user-definable way of declaring the type of an argument or member is solved in C++ by templates or inheritance, those are both not currently understood by FPP. But there exist `serial` ports which can be connected to any other type of port. In theory, this enables the creation of a generic Sensor, however, the details are not documented and we lacked the time to reverse engineer the implementation, which would have been necessary to deserialize the data received by such ports. Thus this section has to be seen as an outline for future work.

\begin{figure}[h]
    \centering
    \begin{tikzpicture}
    \node [state]
    {
        \nodepart[align=center]{one} {\bf ReelayGenericSensor}
        port\_types[10]
        \nodepart{two} - properties\\
        \nodepart{three} {\bf Input Ports:} \\
            serialIn : [10] serial
            {\bf Output Ports:} \\
            reelayEventOut : reelayEvent
        \nodepart{four} {\bf Events:} \\
    };
    \end{tikzpicture}
    \caption{ReelayGenericSensor Component Diagram}
    \label{fig:genericSensor}
\end{figure}


An example instantiation of the generic Reelay sensor might look like this for receiving the data of our simulation component:

```C++
instance reelaySensor1: Ref.ReelayGenericSensor base id 0x0E200 \
   {
      phase Fpp.ToCpp.Phases.configComponents """
        reelaySensor1.set_data_channel_name(0, "speedI", Ref::ReelayDataType::F32);
        reelaySensor1.set_data_channel_name(1, "speedD", Ref::ReelayDataType::F32);
        reelaySensor1.set_data_channel_name(2, "exhaustVelocity", Ref::ReelayDataType::F32);
        reelaySensor1.set_data_channel_name(3, "stage", Ref::ReelayDataType::String);
        reelaySensor1.set_data_channel_name(4, "height", Ref::ReelayDataType::F32);
        """
   }
```

## Alternative designs

As the properties can get quite long there might be a desire to work on files containing the specifications rather than on the individual strings thereof. This could at present be addressed by adding two components. A FileManager component instance would be responsible for the on-disk organization of the specification files and a provided component that loads the individual properties into the `ReelayMonitor` component from the FileManager.

The monitor definition of the `ReelayMonitor` could be supplied at the FPP-component initialization by changing the constants used in the creation of the monitor. An instantiation of a dense-time monitor from the end-user perspective may be along the lines of:

```C++
    instance reelayMonitor: Ref.ReelayMonitor base id 0xE000\
        {
        phase Fpp.ToCpp.Phases.configConstants """
            enum {
                INPUT_T = U32,
                OUTPUT_T = U32,
                MONITOR_TYPE = ReelayMonitor::DiscreteTime
            }
            """
        }
```

Whilst the performance overhead would be relatively minimal the complexity overhead for both the implementors (us) and the users was deemed too high. 




\newpage
# Implementation & Deployment

To show how the components interact and behave in an F' deployment we will show here the details of using the provided components to verify the properties of our rocket example.

## Workflow

The Workflow of monitoring an F' deployment is as follows:

1. Specify the properties to be verified in natural language (e.g., the stages should increase monotonically).
2. Generate the Reelay formulas from the natural language specifications.
3. Determine where the necessary data originates and where it will be required; how many Monitors should there be? What kind of Reelay sensors should be used? Should the JSON events be created directly? Is there an opportunity to use temporal logic instead of ad-hoc methods for determining certain states?
4. The result of (3) is a topology using our provided components to verify the specification from (1).

## !!! Rocket Simulation component

\begin{figure}[h]
    \centering
    \begin{tikzpicture}
    \node [state]
    {
        \nodepart[align=center]{one} {\bf RocketSimulator}
        \nodepart{two} - rocket\_state\\
        \nodepart{three} {\bf Output Ports:} \\
            rocketState : RocketState
        \nodepart{four} {\bf Commands:} \\
            TOGGLE\_SIMULATION() \\
            {\bf Telemetry:} \\
            height : F32 \\
    };
    \end{tikzpicture}
    \caption{RocketSimulator Component Diagram}
\end{figure}

As mentioned in our case the only 'physical sensor' is the rocket simulation. This component is a naive implementation of the rocket model introduced earlier. Every time the scheduling port receives an event, the rocket simulation component updates the state of the rocket and invokes the output ports to send those to the connected component. The integration of the rocket equation is done by a left Riemann sum without regard to numerical stability. The purpose is solely to test the rest of the system. Care must be taken that the time step in the simulation corresponds to the time step of the to `schedIn` connected `rateGroup`. For simplicity this has to be done manually; if the update time value in `run1cycle` in `Top/Main.cpp` is the same numerical value as `rate_group_dt` the numerical time values correspond to milliseconds and the simulation runs in real-time. Then numerical interval values in the Reelay properties correspond to milliseconds. However the rocket time can be configured to move at an arbitrary rate, but care must be taken to change the interval definitions of the properties accordingly as the `ReelayMonitor`'s inserted time value is in milliseconds and the rocket simulation does not provide it's internal time in our implementation. There is no mechanism to catch this discrepancy.


## !!! Build system (0.5 p)

There is a non-trivial amount of CMake used by F' deployments to build deployments. This makes changes to the build settings - even for relatively simple tasks such as keeping debug symbols in the final binary - relatively challenging. Thus we will give a brief outline of how to adapt the CMake files for the deployment to include the Reelay components and their dependencies.


## !!! Running example code output (1p)

When starting `fprime-gds` the simulation is initially turned off. When sending the command `TOGGLE_SIMULATION` the simulation is turned on and the rocket starts to move. The simulation can be turned off again by sending the same command. Once the simulation is started it advances by time-steps `dt`. To change the speed at which the simulation runs one needs to configure the cycle duration of the F' deployment. In our example deployment this means changing the `run1cycle` function inside `Main.cpp` inside the `Top` folder.


## !!! Discussion (1p)

- Performance of ReelayMonitor (time horizon, complexity of property)
- F' particularities / challenges (thoughts on F´)
- due to the way fprime components are instantiated there is less opportunity for compile time optimizations which have overall been a trend in newer C++ editions

As mentioned one of the issues with the monitor-design pattern in FPP is that there is no way for a user to define the type of data that is sent over the `serial` connections and have the compatibility of this declaration verified in regards to the type of data that is actually sent. This issue could be solved in stand-alone C++ using templated classes, but neither F' nor FPP have such a concept, though it would in our opinion greatly ease the creation of reusable components.






# !!! Conclusions and Outlook (1p)

- Brief summary
- What would you pursue next to realize an end to end system? What are interesting avenues for investigation
  - End user has to think about delay introduced when distributing components
  - Delay might be dynamic (relativistic effects cannot be ignored under all circumstances this system might be deoployed in)
    
!!! Another evaluation angle would have involved studying the effect of using ReelayMonitor on an existing F' deployment

Whilst altering the provided components to use R2U2 was considered - and would be reasonably straightforward - it was deemed out-of-scope for this thesis.

As mentioned in [related_work] R2U2 has some distinct advantages over Reelay but all the additional user functionality comes at the cost of complexity which is not coherent with the philosophy of rapid development present in F'.

R2U2 also has a Simulink integration, which is similar in spirit to the integration of reelay with F´ we propose here. Though tapping into the massive Simulink ecosystem certainly has its benefit we feel the downsides of proprietary and costly system outweigh the benefits for our targeted domain.
[](Other major approaches doing RV for aero/space, the FPGA ones, embedded systems etc)

\newpage
\printbibliography
