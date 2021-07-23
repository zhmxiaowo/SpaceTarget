[TOC]


# Quick Start

### 1. Download Space Target plugin
 [Use GitHub: https://github.com/zhmxiaowo/SpaceTarget.git](https://github.com/zhmxiaowo/SpaceTarget.git "Use GitHub: https://github.com/zhmxiaowo/SpaceTarget.git")

 [Download the plugin from the official website](https://www.4dkankan.com/#/developer/downSDK "Download the plugin from the official website")


### 2. Create a AR project
- Start engine 4.26 and create a project **Games -> HandheldAR** .The example names the project **SPTest** and creates the project

<img src=doc/ueimgs/60dc2dd136420.png width = "50%"  alt="image.png"/>

### 3. install Plugin.
- Create the **Plugins** folder in the project root directory,put the **SpaceTarget** plugin folder into the **Plugins** folder.

<img src=doc/ueimgs/60dc37c2cda74.png width = "50%"  alt="image.png"/>

	Note:
	Final path: .../SPTest/Plugins/SpaceTarget
---

- Double click the project folder to open the project again. It will inform rebuild. Click continue

<img src=doc/ueimgs/60dc3892df25c.png width = "50%"  alt="image.png"/>

- Open **Settings -> Plugins** ,and drag the left slider to the bottom to view Others. you can see that spacetarget has been loaded into the engine
(If enabled is not automatically checked, manually check and restart the engine to enable spacetarget)

<img src=doc/ueimgs/60dc3b73de209.png width = "50%"  alt="image.png"/>

### 4. Create a Scene
- We will edit the default scene **HandheldBlankMap** .

<img src=doc/ueimgs/60dc476562928.png width = "50%"  alt="image.png"/>

- Select World Settings.Change **GameMode** to **SpaceTargetGameMode** .Change **Default Pawn Class** to **SpaceTargetPawn** 

<img src=doc/ueimgs/60dc48295b29a.png width = "50%"  alt="image.png"/>

-  **SpaceTargetPawn** Contain AR system starting,if you wanna to use your own GameMode,put the  **SpaceTargetPawn** blueprint node to your own blueprint. **SpaceTargetPawn** Include Camera Component for reciving ARKit ARCore location.


<img src=doc/ueimgs/60dd248ca974c.png width = "50%"  alt="image.png"/>


- In **SpaceTarget Contents/SpaceTarget** ,find **SpaceTargetActor** and drag it into the scene

<img src=doc/ueimgs/60dc4c8d86a69.png width = "50%"  alt="image.png"/>


- Select the **SpaceTargetActor** that was just placed in the scene, and find the DownloadData button in the Details panel, click and download the scene data set.
*How to get data? Please refer [data acquisition](https://www.4dkankan.com/#/developer/useCollection "data acquisition")*
<img src=doc/ueimgs/60dc42a1bf0e0.png width = "50%"  alt="image.png"/>

- Enter the scene code and click download

<img src=doc/ueimgs/60dc42d754bba.png width = "50%"  alt="image.png"/>

- Please be patient and wait for the downloading. After that,if Engine tips import window,choose  **Don't Import** . the  **Content/SpaceTarget folder**  will be generated under Content folder(SPTest), which contains the data of the download scene. 

<img src=doc/ueimgs/60ddcb1246dc5.png width = "50%"  alt="image.png"/>

-  **RE** Select **SpaceTargetActor** in the scene ,find **Data** drop-down box,and select the scene id just imported

<img src=doc/ueimgs/60dc4724a929d.png width = "50%"  alt="image.png"/>


At this point, we can see that our scene data has been loaded.

<img src=doc/ueimgs/60dc46dc82930.png width = "50%"  alt="image.png"/>

### 5. Custom content 
- Place the content in the scene, and place the content under **SpaceTargetActor** as a sub object. The example places three cube of different colors.

<img src=doc/ueimgs/60dc4a221ffd3.png width = "50%"  alt="image.png"/>

- Here we choose to hide the display of the scene model

<img src=doc/ueimgs/60dc4aaf799d0.png width = "50%"  alt="image.png"/>

	Note:
	The Blueprint of SpaceTargetActor contains the entire SpaceTarget startup nodes and event binding nodes.
### 6. Package scene 
- After packaging, you can restore the 3 cubes in the real scene .


<img src=doc/ueimgs/60dd7e7674168.png width = "50%"  alt="image.png"/>


<img src=doc/ueimgs/60dd7ff416e2b.gif width = "30%"  alt="tt0.top-965488.gif"/>



How to configure the packaging scene? Please refer to the official Epic Game documentation  [iOS game development ](https://docs.unrealengine.com/4.26/zh-CN/SharingAndReleasing/Mobile/iOS/ "iOS game development ")    [Android game development](https://docs.unrealengine.com/4.26/zh-CN/SharingAndReleasing/Mobile/Android/ "Android game development")

### 7. Custom supplement
- We provide two **Delgate** that can bind callbacks when the scene is tracking and lost 

<img src=doc/ueimgs/60dd2c96b835a.png width = "50%"  alt="image.png"/>


	Note:
	The SpaceTargetActor in the example is bound :
	1. When the scene is tracking, the SpaceTargetActor and its child objects are displayed
	2. When the scene is lost, hide the SpaceTargetActor and its children 


- SpaceTargetActor property details 

<img src=doc/ueimgs/60ddc78ce2ceb.png width = "50%"  alt="image.png"/>

| property |  property |
| ------------ | ------------ |
| Data  | Scene list selection box, re-select Actor to refresh this table, used to change different scene data   |
|  Download | Download button, click to open download panel |
| Data Base  | Internal properties, change with Data   |
|  Center Mode | Target: SpaceTargetActor scene data is static, change the relative position of Camera to match. Device: SpaceTargetActor changes relative position with the camera   |
| Visible Model | Show or hide the current scene Mesh, use True for editing,False for shipping |


