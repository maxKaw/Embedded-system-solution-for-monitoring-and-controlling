# Brogrammersv2

DISTRIBUTED AND EMBEDDED SYSTEMS


Your team are to prototype an embedded system (hardware and software/firmware) for operation as an environmental monitoring and control system. It must be capable of monitoring any 2 of these 3 variables: 
•	**Ambient Humidity (e.g. DHT11/22 sensor) 
•	Ambient Temperature (e.g. a thermistor or sensor as above) 
•	Air Safety / CO relative levels (e.g. a MQ-7 / MQ-x sensor) **

It also needs the capability to detect human presence (e.g. a HC-SR501 PIR sensor) for some features. 
 
Output requirements for the prototype start with reasonably simple requirements – effectively a status light or LED. There are a number of “acceptable regions” for the various environmental quality indicators – green and amber, indicating the acceptability of a reading. 
 
Variable 	Green Region 	Amber Region(s) 
Humidity (%) 	35-60% 	Low >=25%, High<=75% 
Temperature (C) 	18-23 deg C 	Low >=16,    High <=27 degrees C 
Air: Carbon Monoxide parts-per-million 	< 9 ppm CO 	<= 25 ppm CO 
If all sensors indicate values in the green region, this is considered ideal, and a green light should be shown.  If any sensor indicates a reading outside of the green region, but within the wider amber region, then an amber light should be shown. If any sensor indicates a reading outside of both the green and amber regions, then this is a serious situation needing attention - a red light should be shown.  
 
Beyond this, additional features introduce a requirement to display certain formatted data values on an output screen (e.g. an LCD character or OLED pixel display), and produce audio alerts. 
 
The client has specified that they foresee an Internet of Things (IoT) development pathway, so the system will be able to transmit its status regularly to a server that will facilitate remote monitoring. You can assume the system has access to a WiFi access point (but you are responsible for provisioning it). PHP source code for a simple server endpoint will be provided for testing purposes, though again, you must provision/host this. You are welcome to edit this code and to add functionality (e.g. visualisation / review) as required. 
 
Documentation and Overview 
The key hardware requirement is that the microcontroller (MCU) hardware is centred on an Arduinocompatible board. This means you can consider the entirety of the Arduino range, though an Uno is likely to be acceptable for basic development only. You are also invited to look at the Espressif ESP8266 and ESP32 boards; with built-in WiFi and an Arduino core SDK, they can be programmed almost transparently in Arduino IDEcompatible environments (e.g. Sloeber, Visual Micro, PlatformIO). 
 
The developed and submitted software must be developed primarily using Arduino standard libraries (or closecompatible alternatives in the case of the ESP boards) and C or C++. You may use other third-party libraries/drivers but these must be clearly identified in your submission, and clear instructions must be included for access to the Software Development Kit (SDK)’s documentation. You will have access to componentry for sensing/actuation, though may (at your own cost) include other peripherals, but please ensure you familiarise yourself with the physical and software connectivity requirements. Try to assess how much General Purpose Input/Output (GPIO) capacity your components will require and how to talk to them! Some environmental sensing technology is very expensive; please see the note “SPECIALIST HARDWARE”. 
As such, your design documentation will begin with a technical review of relevant interconnection architectures and protocols, evaluating and outlining those believed useful for this project. You should also evaluate how each interconnection technology is supported by your host MCU, and built-in/library support or any difficulties envisaged. 
 
You will need to include all specific GPIO information as required for your planned prototype system in your architectural overview design documentation. Your system architecture / interconnect diagram should clearly identify key details (see tasks below) for external input/output. This is a crucial first-step validation that your chosen MCU can support all of your input and output requirements. 
 
**IMPORTANT: In the interests of electrical safety, all peripherals will operate from the MCU/Arduino’s plugin power supply only (this effectively limits you to using a maximum 12V, 5V or 3.3V depending on board and power source). Please see the note “SPECIALIST HARDWARE” to avoid unnecessary expense. **
 
A key stage in modelling your software/firmware design is to produce a Finite State Machine (FSM) that describes the behaviour of your software’s main loop and any key sub-states. It is recommended that you use a UML State Machine Diagram to represent this, though any clear state machine diagram will be assessed. 
 
Feature Set - Your group is to produce (design and implement) a prototype embedded system and software capable of performing the following features. You should ensure that your software supports as many of the following features are you can (this list broadly increases in difficulty): 
* A.	Power-on Self-Test – ensure all expected componentry is connected and behaving as expected (where possible), that any sensors’ “stabilisation” or warm-up period has elapsed, and then report to the MCU’s serial/debug port a message that the system is ready and working. 
* B.	Read sensors at a rate appropriate for each sensor, and determine which indicator (green, amber or red) is shown, based on the table outlined previously. Whenever a variable causes a change from/to GREEN, this should be logged with a debug message identifying the variable on the MCU’s serial/debug port. 
* C.	Building on the functionality above; every 5s, concatenate a simple debug string (containing current/latest values for all sensor readings/statuses) and print this via the MCU’s serial/debug port. 
* D.	Determine (e.g. a PIR sensor) if the monitored building is occupied. It should be considered vacant 10 minutes after the last movement, and should be considered occupied as soon as motion is detected. This information should be added to the feature C debug string; additionally, each time the status changes, this event should be logged also. 
* E.	Readings must now be stored every 5s in volatile memory. If a connected push-button is held down for ~1s, the system should cycle between output intervals [5s, 10s, 30s, 60s, 2mins, 5mins]. Output is still only required for the latest values for each sensor. Each time the output interval is changed, this should be communicated via a debug string (e.g. “Output interval is now 30s”) to the MCU’s serial/debug port. 
* F.	Transmit the volatile-stored readings at the regular output interval [though with a minimum interval of 30s – 
* i.e. at most twice a minute] determined in the previous features to a server for remote storage. 

 
 	 
**Advanced Features:**
* G.	If the building is determined occupied, as per feature D, then the system should sound an audible alert (min 1s duration) as the table below. The alert can be “snoozed” for 2m by tapping a push button following the  audible alert. This must only snooze an existing alert: a new/escalated cause should start a new audible alert. Alert statuses (including snooze) should be logged as per feature C, and at each status change. 
* System 	status 	== AND… 	occupied 	Audible alert every 
* Amber for any sensor 		30s 
* Red for any sensor 		5s 
* H.	Store readings in a plain-text file on an SD/MMC card for local, separable, storage. Each reading should be timestamped with an offset from a given local epoch. You should name/date the files based on an epoch time/datestamp provided when you communicate with the remote server in the previous feature. To minimise flash wear, log files should be written only every 2 minutes. Only once a batch has been verified as stored on the non-volatile card, it can be freed from local volatile memory. 
* I.	Showcase challenge feature: one additional feature (set) of your choosing that showcases a specific technique or technology outlined in the module. Please discuss your plans with the module leader before you embark on development. Example areas include (but are not limited to) power conservation/energy efficiency, advanced user output/input technologies, inclusion of a real-time clock (RTC). If you are stuck for ideas or would like a benchmark for complexity, an example showcase feature is described below:  
* • Display readings and system status in real time on a directly-driven, text-based display. The display should be updated as frequently as readings are obtained; not as per the (feature E) output interval. Steps should be taken to eliminate or reduce display flicker. To avoid display overcrowding, your display should have minimum 2 distinct modes – they should cycle based on i) time, or ii) a push-button press. Displaying “alert” details (feature G) should always take priority when an alert is active or snoozed. 

 
***---SPECIFICATION CONTINUES WITH TASKS OVERVIEW--- ***
 	 
**Tasks:** your group are required to complete the following tasks as part of this system development. It is recommended that you use this nomenclature and numbering as headings in your associated report.  
Design: 


1. 1.Review of interconnect technologies and protocols – a technical review of available interconnection architectures and protocols, evaluating and outlining those believed useful or appropriate for this project (based on known component needs). You should also evaluate how each discrete interconnection technology is supported by your host MCU, and built-in/library support or any difficulties envisaged in using this technology. This should clearly reference source datasheets and Application Programming Interface (API) documentation where relevant.
1. 2.System architecture / interconnect diagram and overview – a clear overview diagram that communicates the specific componentry and GPIO information as required for your planned prototype. 

*This should include, at a minimum, for each external input/output: *
* a.details of each external input / output component / peripheral 
* b.connection detail between host MCU/peripheral; protocol information, and key data values 




1. 3.Finite State Machine(s) to describe system (main loop) logic – a detailed description of stateful logic for your prototype, with a clear indication of valid states, transitions between those states, and causal events. This should, at a minimum, outline the states managed by your system’s main loop. You may use any clear state machine diagram notation, but UML State Machine Diagrams are recommended.   


**Implementation / Prototyping:** 
1. 4.Hardware Design / Breadboard Prototype – You will need to assemble a minimum of one hardware prototype capable of fulfilling the feature set you plan to implement (note SPECIALIST HARDWARE, below). This should be evidenced by an overall schematic and/or breadboard view of the hardware MCU and its connected componentry, along with a photograph of any versioned breadboard setups. It is recommended, in terms of managing complexity, that you adopt an incremental, versioned approach to hardware prototyping (see Versioning, below).  
1. 5.Software Implementation – You should, in accordance with an appropriate hardware prototype from the previous step, and the earlier software design stages, implement a prototype that fulfils a subset of the feature set. This is evidenced by submitting the source code (ideally via a group SCM). Further, it is recommended, in terms of managing complexity, that you adopt an incremental approach in developing software (see Versioning, below). 
1. 6.Versioning / SCM Repository – as with several other Software Engineering assignments, you are expected to utilise a Source Control Management (SCM) repository for this task, ensuring the module leader has full access. You are also expected to maintain a rudimentary versioning system for hardware and software prototypes. At minimum, this should prevent mismatching a software’s feature set to a hardware prototype with insufficient capabilities (e.g. software supports SD card file storage, but the prototype has no SD card slot!). This needn’t include automated hardware verification – it is acceptable that a human checks version numbers match (e.g. SW version 1.34-F requires hardware version F) 


**Evaluation:**
1. 7.Basic Functional / Acceptance Testing – You will devise, complete and log a series of functional test cases aimed at testing all aspects of system functionality. You are advised to carefully plan out your testing scripts; noting the number of features that have approximate timing constraints and how to verify their functionality reliably and efficiently. You may wish to use one of your testing scripts as a basis for your demonstration / functionality walkthrough (see below). 
1. 8.Further / Other Testing – You will devise a series of further tests for your system that permit you to verify and validate additional aspects. This should comprise a range of white-box and black-box testing approaches for functional criteria and tests designed to examine non-functional requirements. You are required to research beyond the module’s notes; examining approaches to conducting tests and metricising outcomes (i.e. how reliable is something). This may include, but is not limited to: power consumption, reliability under various conditions / over time, behaviour under fault conditions, etc. You may examine and report on both software and hardware issues. If you find failings that are impractical to resolve during prototyping, then consider and document how they might be fixed for a production system. 
1. 9.Demonstration – Your team will produce and deliver a short presentation, which will culminate in a demonstration of your prototype hardware/software’s working features. You will be expected to identify any faulty or part complete areas – and be prepared to take questions on particular areas of functionality, development and your project’s approach. You may wish to plan an order of events for your demonstration, bearing in mind that some features have time constraints attached to them. 
