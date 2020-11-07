# Recording Sessions

The **Matinee** animation tool provides the ability to animate the properties of Actors over time, to create either dynamic gameplay or cinematic in-game sequences. The system is based on the use of specialized animation tracks in which you can place keyframes to set the values of certain properties of the Actors in the level. The Matinee Editor is similar to the non-linear editors used for video editing, making it familiar to video professionals.

The **Sequence Recorder** allows you to capture specified Actors during level editing or gameplay that can be saved as a new **Level Sequence**, and can be edited inside of **Sequencer**. This is useful for quickly capturing content for scenes, as you can take a playable character, perform some actions during gameplay while recording with the Sequence Recorder, then take that data into Sequencer, creating a cinematic around it.

In this example, we will use the Sequence Recorder to record our playable character's movement, which we can then edit.

1. From the **Menu Bar** under **Window**, select **Sequence Recorder**.

   ![RecorderStep1.png](https://docs.unrealengine.com/Images/Engine/Sequencer/HowTo/SequenceRecorder/RecorderStep1.jpg)

2. The Sequence Recorder window will automatically open. There are some options under **Sequence Recording** which will determine how \(and where\) the new Level Sequence asset will be saved. You can choose to record Actors that are spawned \(such as particle effects, other characters, etc.\), determine the naming convention and save location, and change other settings like the duration to record or delay before starting to record.
3. From the Main Editor Toolbar, click the **Play** button to start a Play in Editor \(PIE\) session.
4. Press **Shift+F1** to gain mouse control once inside the game.
5. On the **Sequence Recorder** window, click the **Add** button.

![](../.gitbook/assets/image%20%289%29.png)

From the available, dashboard under `/Cinematics`  the folder you can control various parameters   
like:

1. Car lights, Headlights
2. Environmental Factors
   1. Fog
   2. Sky-light flux
   3. Sun position
3. Landscape visibility
4. object visibility

**Sequence Recorder can track any object for tracking and can manage tracking of assets centrally.**

