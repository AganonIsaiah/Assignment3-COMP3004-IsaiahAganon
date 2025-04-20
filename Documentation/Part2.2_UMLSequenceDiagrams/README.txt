TESTING INSTRUCTIONS

Sequence Diagram 1.1 (Normal Event)
- Single person requests an elevator at floor 5, destination floor 100

Input:
    Number of Passengers: 1
    Number of Floors: 100
    Number of Elevators: >= 1

    Passenger ID: 1
    Request Car:
        Time Step: 0
        Floor: 5
    Exit Car:
        Time Step: 1
        Floor: 100

Sequence Diagram 1.2 (Normal Event)
- Multiple people on different (randomized) floors, going to different (randomized) destinations

Input:
    Number of Passengers: > 1
    Number of Floors: > 1
    Number of Elevators >= 1

Sequence Diagram 1.3 (Safety Event)
- Passenger presses the help button during normal operations

Input:
    Number of Passengers: >= 1
    Number of Floors: >= 1
    Number of Elevators: >= 1

    Help Alarm:
        Time Step: >= 0

    or 

    Passenger ID: 1
    Push Help:
        Time Step: >= 0
        Floor: >= 0

Sequence Diagram 1.4 (Safety Event)
- System triggers a power out alarm, after some normal operations

Input:
    Number of Passengers >= 1
    Number of Floors: >= 1
    Number of Elevators: >= 1

    Fire Alarm:
        Time Step: >= 0