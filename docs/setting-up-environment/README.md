# Setting Up Environment









Unreal engine arranges the environments in form of the MAPS. Much like **Arrays** and **Sets**, Blueprint Maps have a variety of uses when developing a game. If you're unfamiliar with the term "container"; think of a storage box, where you label items and place them inside of the box for immediate \(or later\) use. If you've used Arrays to store and work with collections of items, you've already started using Blueprint containers in UE4. For example, when using Arrays to store items for later use, the item's label is its place in the array. Maps are similar to Arrays in that they both use a label to indicate where the item is located in the container, however, labels for Maps are different from those used for Arrays. When using Arrays, the label is the item's sequential index in the container, whereas, when using Maps, the label is a key that's associated with the item in the container. Having the ability to associate items in a container with their respective keys, enables developers to efficiently lookup and retrieve items with the use of the keys, and so harness Maps in creative ways.

> For illustrative purposes, we're using a **Blank Project** \(with the highlighted settings\) to show you how to create and edit Blueprint Maps.  
>
>
> ![sampleProject.png](https://docs.unrealengine.com/Images/Engine/Blueprints/UserGuide/Maps/sampleProject.jpg)![sampleProject1.png](https://docs.unrealengine.com/Images/Engine/Blueprints/UserGuide/Maps/sampleProject1.jpg)

### **Adversarial Engine Has 2 Maps**

1. Environment -1**:** 
   1. Has tracks as shown in the research paper
   2. It has objects like animals, wooden logs, trees, shrubs, roads, cars, trucks, buildings etc. 
   3. Can be used for the repetitive control environment experiment.    
2. Environment -2: Can be used for more detailed scenarios. This map has a variety of additional assets like
   1. Water bodies
   2. Over bridges
   3. Sky scrappers
   4. billboards
   5. Variety  of 2 and 3 wheelers

### Loading Maps

By default, Maps are present in the`content/map`directory.

Load any of the map of choice 

\*\*\*\*

