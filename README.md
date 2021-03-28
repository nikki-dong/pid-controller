# pid-controller

PidController class implements basic functions of a PID controller, that is widely used in various engineering fields.

It has minimal dependency, and is agnostic to how to get current system time, since users will supply callbacks to it. This enables easy mocking and testing.

It is also templated, so users can decide what data types to work with, that might benifit some small embedded systems.
