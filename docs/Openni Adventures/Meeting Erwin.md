
## 21-11-2022

- Want to start finishing my thesis
- I don't see an end coming closer
- I have been working on preliminaries for such a long time
- Current version still not working fully
	- Multiple Point clouds not being shown
	- Still working on recording and playback
	- Still need a way to reliably create a ground truth 
	- Idea:
		- Go through every frame and show 3 different angles
		- Make the user select every joint
		- Using multiple viewing angles an exact location of the joint can be determined
- New scope maybe:
	- The effect of multi modal cameras on the 2 most promising
		- Template Based Methods
		- Feature Based Methods
		- Machine Learning based models
	- How many recordings should I create?
	- How much overlap should there be between cameras?
	- Some methods improve using multiple frames how should I consider this in the measurement?
		- Should single frame performance be important?
			- Most RGB-D applications are not still images
		- Should I use the average accuracy?
		- Should I use a delayed average for the methods that improve over time?
	- How big should the dataset be?
	- Can I use pretrained models?
	- How can I create ground truth fast? 
		- Is there a better way other than manual?
	- If I have to manually create the ground truth this will take another couple months, could we somehow reformulate the problem so it only takes another 2 months?
 
## 19-12-2022

- What to work on:
	1.  Playback RS
	2. Recording Orbbec
	3. Playback Orbbec
	4. Skeleton Detection in recording stream
- Research Focus
	- When does HPE go wrong?
	- Detect Scenarios where it goes wrong.
	- Dataset contains of depth and rgb stream and skeleton ground truth
	- 