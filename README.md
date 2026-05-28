# maze-solving-robot
Arduino maze-solving robot project for Scenario B ELEC0035 UCL

## 1. Development Process
```text
Day 1: Read brief and clarify tasks
  ↓
Day 1: Each member develops their subsystem
  ↓
Day 2: Agree uniform interfaces for sensors, motors, encoders, and decision logic
  ↓
Day 2: Role 1 builds the software framework
  ↓
Day 3: Connect subsystem code into the main program
  ↓
Day 3: Dry run with wheels raised and sensors tested
  ↓
Final: Test in maze attempt 1
  ↓
Final: Improve and test in maze attempt 2
  ↓
Submit peer review
```
## 2. State Transition Diagram
<img width="333" height="315" alt="image" src="https://github.com/user-attachments/assets/71426a11-4d9e-4353-98ff-8dd7bd62e035" />

```text
R=1 indicates that the robot is in a square with a section of the red walls in front.
G=1 indicate that the robot is in a square adjacent to one of the green walls.
```
<img width="484" height="304" alt="image" src="https://github.com/user-attachments/assets/d2fd882f-0db2-47c2-ba22-3066f42dc209" />

## 3. FlowChart
```text
1.Explaiination:
  There's no wall ahead
  → Keep going
  There is a wall ahead
      Is there a blockage on the left?
        Yes → Move to the right
        No → Move left

2. use voting algorithm to solve "an unstable front wall is detected";
3. search left for a gap;
4. Polling to avoids being confused by the comb-shaped irregularities on the front wall;

```
<img width="556" height="701" alt="image" src="https://github.com/user-attachments/assets/2ca0fce4-e264-402d-88cc-071f0db88261" />




