# Embedded System solution for monitoring and controlling 

University project. Achieved grade 87%.

# Detail of the task

Your team are to prototype an embedded system (hardware and software/firmware) for operation as an environmental monitoring and control system. It must be capable of monitoring any 2 of these 3 variables: 
- Ambient Humidity (e.g. DHT11/22 sensor) 
-	Ambient Temperature (e.g. a thermistor or sensor as above) 
-	Air Safety / CO relative levels (e.g. a MQ-7 / MQ-x sensor)

It also needs the capability to detect human presence (e.g. a HC-SR501 PIR sensor) for some features. 
 
Output requirements for the prototype start with reasonably simple requirements – effectively a status light or LED. There are a number of “acceptable regions” for the various environmental quality indicators – green and amber, indicating the acceptability of a reading. 
<table style="width:100%">
<tr>
<th>Variable</th> 	<th>Green Region</th> 	<th>Amber Region(s)</th>
</tr>
<tr>
<td>Humidity (%)</td> 	<td>35-60%</td> 	<td>Low >=25%, High<=75%</td> 
</tr>
<tr>
<td>Temperature (C)</td> 	<td>18-23 deg C</td> 	<td>Low >=16,    High <=27 degrees C</td> 
</tr>
<tr>
<td>Air: Carbon Monoxide parts-per-million</td> 	<td>< 9 ppm CO</td> 	<td><= 25 ppm CO</td> 
</tr>
</table>
If all sensors indicate values in the green region, this is considered ideal, and a green light should be shown.  If any sensor indicates a reading outside of the green region, but within the wider amber region, then an amber light should be shown. If any sensor indicates a reading outside of both the green and amber regions, then this is a serious situation needing attention - a red light should be shown.  
 
Beyond this, additional features introduce a requirement to display certain formatted data values on an output screen (e.g. an LCD character or OLED pixel display), and produce audio alerts. 
 
The client has specified that they foresee an Internet of Things (IoT) development pathway, so the system will be able to transmit its status regularly to a server that will facilitate remote monitoring. You can assume the system has access to a WiFi access point (but you are responsible for provisioning it). PHP source code for a simple server endpoint will be provided for testing purposes, though again, you must provision/host this. You are welcome to edit this code and to add functionality (e.g. visualisation / review) as required. 

# Feature Set

Your group is to produce (design and implement) a prototype embedded system and software capable of performing the following features. You should ensure that your software supports as many of the following features are you can (this list broadly increases in difficulty): 

- Power-on Self-Test – ensure all expected componentry is connected and behaving as expected (where possible), that any sensors’ “stabilisation” or warm-up period has elapsed, and then report to the MCU’s serial/debug port a message that the system is ready and working. 
- Read sensors at a rate appropriate for each sensor, and determine which indicator (green, amber or red) is shown, based on the table outlined previously. Whenever a variable causes a change from/to GREEN, this should be logged with a debug message identifying the variable on the MCU’s serial/debug port. 
- Building on the functionality above; every 5s, concatenate a simple debug string (containing current/latest values for all sensor readings/statuses) and print this via the MCU’s serial/debug port. 
- Determine (e.g. a PIR sensor) if the monitored building is occupied. It should be considered vacant 10 minutes after the last movement, and should be considered occupied as soon as motion is detected. This information should be added to the feature of debug string; additionally, each time the status changes, this event should be logged also. 
- Readings must now be stored every 5s in volatile memory. If a connected push-button is held down for ~1s, the system should cycle between output intervals [5s, 10s, 30s, 60s, 2mins, 5mins]. Output is still only required for the latest values for each sensor. Each time the output interval is changed, this should be communicated via a debug string (e.g. “Output interval is now 30s”) to the MCU’s serial/debug port. 
- Transmit the volatile-stored readings at the regular output interval [though with a minimum interval of 30s – 
* i.e. at most twice a minute] determined in the previous features to a server for remote storage. 
- If the building is determined occupied, as per feature D, then the system should sound an audible alert (min
1s duration) as the table below. The alert can be “snoozed” for 2m by tapping a push button following the
audible alert. This must only snooze an existing alert: a new/escalated cause should start a new audible
alert. Alert statuses (including snooze) should be logged as per feature C, and at each status change.

<table style="width:100%">
<tr>
 <th>System 	status 	== AND..</th><th>occupied 	Audible alert every</th>
</tr>
<tr>
<td>Amber for any sensor</td> 		<td>30s</td> 
</tr>
<tr>
<td>Red for any sensor</td> 		<td>5s</td> 
</tr>
</table>

- Store readings in a plain-text file on an SD/MMC card for local, separable, storage. Each reading should be timestamped with an offset from a given local epoch. You should name/date the files based on an epoch time/datestamp provided when you communicate with the remote server in the previous feature. To minimise flash wear, log files should be written only every 2 minutes. Only once a batch has been verified as stored on the non-volatile card, it can be freed from local volatile memory. 
- Showcase challenge feature: one additional feature (set) of your choosing that showcases a specific technique or technology outlined in the module. Please discuss your plans with the module leader before you embark on development. Example areas include (but are not limited to) power conservation/energy efficiency, advanced user output/input technologies, inclusion of a real-time clock (RTC). If you are stuck for ideas or would like a benchmark for complexity, an example showcase feature is described below:  
- Display readings and system status in real time on a directly-driven, text-based display. The display should be updated as frequently as readings are obtained; not as per the output interval. Steps should be taken to eliminate or reduce display flicker. To avoid display overcrowding, your display should have minimum 2 distinct modes – they should cycle based on i) time, or ii) a push-button press. Displaying “alert” details should always take priority when an alert is active or snoozed. 
