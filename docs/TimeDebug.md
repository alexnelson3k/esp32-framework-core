# Timing Debug

```
set time_debug true
set time_debug_warn_threshold_us 5000
set log_level 2
```

Both timing settings retain FLAG_SAVE_TO_FLASH and are saved through the existing storage mechanism. Their defaults are false and 5000 microseconds. Old profiling_enabled and profiling_warn_threshold_us names are removed from commands and API. Existing numeric parameter indexes are unchanged; storage accepts the previous checksum only when the two timing names are the sole registry change, updates the header, and loads existing values without resetting other settings.

When enabled, a module taking more than the threshold produces a WARN message:

```
[WARN][TimeDebug] FermDisplay loop execution time: 12000 us
```

The threshold is strictly greater-than, as before. Timings cover each enabled registered module and the separately serviced Logger and TimeManager. Disabled modules are not measured by ModuleManager. Each measurement ends before its own diagnostic message is printed.

The complete frameworkLoop produces an INFO message on every enabled iteration, regardless of the module threshold:

```
[INFO][TimeDebug] Total loop execution time: 14500 us
```

Total time includes Logger and TimeManager servicing, registered modules, event handling, storage processing and any module timing messages. It excludes printing the total-time message itself and work outside frameworkLoop. INFO output requires log_level >= 2; WARN requires log_level >= 1. Destination switches and Syslog's independent verbosity filter still apply.

```
set time_debug false
```

This stops both module and total timing reports. It does not suppress unrelated device Serial.print statements.
