# Processor

The Processor consists of two parts. It processes complex events described by input.
In this case files with command and control actions for the vehicles.

The part that processes files is the notifier

The processor can have many notifiers and each notifier can be a separate thread.
The notifiers can be defined in the configuration file (VehicleProcessor.cfg)

## Notifiers

The notifier is watching a directory structure for new input files to arrive and as soon as there is one, this will be processed by the thread per notifier directory. The output is also defined in the configuration file.

Many directories can be watched, each in a separate thread.
