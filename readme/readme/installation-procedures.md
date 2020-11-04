---
description: Adversarial simulator installation procedure.
---

# Installation Procedures

Adversarial simulator depends on two components 1\) Unreal engine 2\) Nvidia Drive SDK

As the Nvidia Drive SDK is only compitible with ubuntu - 18.04 it is advisable to install the unreal engine and Nvidia Drive SDK on to ubuntu. If you prefer to install the unreal engine over windows then you will be able to only batch process output of Unreal Engine to Nvidia Drive SDK.

{% tabs %}
{% tab title="Install Unreal Engine" %}


## 1.  On Windows

1. [Create an Epic Games Account \(if you don't already have one\)](https://docs.unrealengine.com/en-US/GettingStarted/Installation/index.html#creatinganepicgamesaccount)
2. [Download and Run the Installer \(Epic Games Launcher Set-Up Program\)](https://docs.unrealengine.com/en-US/GettingStarted/Installation/index.html#downloadingandrunningtheinstaller)
3. [Sign into the Epic Games Launcher](https://docs.unrealengine.com/en-US/GettingStarted/Installation/index.html#signingintotheepiclauncher)
4. [Install Unreal Engine](https://docs.unrealengine.com/en-US/GettingStarted/Installation/index.html)
5. [Launch Unreal Engine](https://docs.unrealengine.com/en-US/GettingStarted/Installation/index.html#launchingunrealengine)\*\*\*\*

## **2.  On Linux**

Currently, Unreal **** don't supply a binary installer of UE4 for Linux users. The good news is that you can compile a binary of Unreal Engine from the same source code that we use to develop the engine. If you want to access our source code, you'll have to register for an Epic Games account, sign up for a GitHub account, associate your GitHub username with your Epic Games account, and set up Git on your Linux machine.

### 1. Registering for an Epic Games Account

If you want to create games with Unreal Engine or contribute to the future of UE4 on Linux, you'll need to have a valid Epic Games account.

1. Navigate to [UnrealEngine.com](https://www.unrealengine.com/what-is-unreal-engine-4) .
2. Click **Get Unreal**.
3. Fill-out our **'Join the Community'** form and click **Sign Up**.

   ![Create\_Account-1.PNG](https://docs.unrealengine.com/Images/Platforms/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/Create_Account-1.jpg)

4. Read our [Unreal Engine End User License Agreement](https://www.unrealengine.com/eula) \(EULA\) prior to clicking **Accept**. Refer to our [EULA FAQ](https://www.unrealengine.com/faq#legal) if you have additional questions about the EULA's terms and conditions.

   ![EULA-1.PNG](https://docs.unrealengine.com/Images/Platforms/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/EULA-1.jpg)

There are many benefits for having an account with us, including \(but not limited to\) access to the Unreal Engine community, where you'll connect with other UE4 Linux developers, and where you'll be able to keep up with the latest news about Unreal Engine.

#### Setting up Git <a id="settingupgit"></a>

Now that you've registered for an Epic Games account, you're ready to work through the following steps to gain access to the UE4 source code on GitHub.

1. Sign up for a GitHub account at [GitHub.com](https://github.com/EpicGames) .

   ![GItHub\_Header.PNG](https://docs.unrealengine.com/Images/Platforms/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/GItHub_Header.jpg)

2. Go to [your Epic Games account](https://accounts.unrealengine.com/login) and click **Connected Accounts**.  


   ![Connected\_Accounts-1.PNG](https://docs.unrealengine.com/Images/Platforms/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/Connected_Accounts-1.jpg)

3. Link your GitHub username by clicking **Connect** under the GitHub icon.
4. To authorize the connection click **Authorize EpicGames**.

**Section Result**

By now, you've registered for an Epic Games account and linked it with your GitHub username. Also, you've installed Git and you're ready for the next step, where you'll fork and clone the UE4 source code from the Unreal Engine GitHub repository.  


### 2 - Downloading UE4 on Linux

Now that you've set up Git on your Linux machine, you're ready to download the Unreal Engine source code from our [GitHub site](http://github.com/EpicGames) . There are a couple of ways to download the UE4 source code, one way is to use the download feature on our GitHub page, and the other way is to clone the Unreal Engine repository with your Git client. We'll cover both ways on this page.

#### Downloading from GitHub <a id="downloadingfromgithub"></a>

If don't want to clone the Unreal Engine repository with Git, you can use the **Download ZIP** button on our [GitHub site](http://github.com/EpicGames) .

1. First, select the branch you want to download by clicking **Branch:** on the left side of the page. For more info on branches in the Unreal Engine GitHub repository, please see the Source Branches Reference page.

   ![SelectRepoBranch.png](https://docs.unrealengine.com/Images/Platforms/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/SelectRepoBranch.jpg)

2. After selecting the branch you want to download, click Clone or download, which is located on the right side of the page.

   ![CloneorDownloadButton.png](https://docs.unrealengine.com/Images/Platforms/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/CloneorDownloadButton.jpg)

3. Click **Download ZIP** to download the Unreal Engine source code.  


   ![DownloadSourceButton.png](https://docs.unrealengine.com/Images/Platforms/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/DownloadSourceButton.jpg)

4. Finally, decompress `UnrealEngine-(branch_name).zip` onto your hard disk.



#### Cloning with Git <a id="cloningwithgit"></a>

If you're new to cloning with Git, refer to [GitHub's Cloning Guide](https://help.github.com/articles/cloning-a-repository/#platform-linux) before executing the following steps.

1. Click the **Unreal Engine** link to access our [repository](https://github.com/EpicGames/UnrealEngine) .

   ![Git\_Repository-1.PNG](https://docs.unrealengine.com/Images/Platforms/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/Git_Repository-1.jpg)

2. Click **Clone or download**.  


   ![CloneorDownloadButton.png](https://docs.unrealengine.com/Images/Platforms/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/CloneorDownloadButton.jpg)

3. Click the **Copy URL** button to copy the clone URL for the repository.

   ![CopyHTTPS.png](https://docs.unrealengine.com/Images/Platforms/Linux/BeginnerLinuxDeveloper/SettingUpAnUnrealWorkflow/CopyHTTPS.jpg)

4. Open Git Bash, type `git clone`, and paste the URL you copied in the previous step.
   1. Clone EpicGames/UnrealEngine repository

      `git clone https://github.com/EpicGames/UnrealEngine -b release`

   2. Run Setup.sh once.

      `cd UnrealEngine`

      `./Setup.sh`

      The script will try to install additional packages \(for certain distributions\) and download precompiled binaries of third party libraries. It will also build one of the libraries on your system \(LinuxNativeDialogs or LND for short\).

      You should see \*\* SUCCESS \*\* message after running this step. If you don't, take a look into BuildThirdParty.log located in Engine/Build/BatchFiles/Linux directory.

   3. After the successful setup, you can generate makefiles \(and CMakelists.txt\).

      `./GenerateProjectFiles.sh`
{% endtab %}

{% tab title="Nvidia Drive SDK" %}
The NVIDIA SDK Manager gives access to all the necessary software for all active NVIDIA DRIVE™ development platforms. Please follow the steps below to get the latest version available for your hardware.

**Upgrade to DRIVE Software 10.0 now! Here’s what you need to know:**

* Your Host PC must be running Ubuntu 18.04
* Users currently running DRIVE Software 9.0 can upgrade directly to DRIVE Software 10.0
* Users currently running DRIVE Software 8.0 will first need to upgrade to DRIVE Software 9.0 \(available in the [archive section](https://developer.nvidia.com/drive/downloads#CollapseZero) below\) before upgrading to DRIVE Software 10.0
* Downgrading from DRIVE Software 10.0 to DRIVE Software 9.0 is not supported by default. If you have a critical need to downgrade, please [contact your NVIDIA representative](https://www.nvidia.com/en-us/self-driving-cars/#self-driving-contact-modal)
* For further instructions on how to upgrade DRIVE Software, please refer to the [DRIVE Documentation](https://docs.nvidia.com/drive/drive_os_5.1.6.1L/drive-qsg/index.html)

Install through SDK manager  
      [Install through NVIDIA SDK Manager](https://developer.nvidia.com/nvsdk-manager)      [  DRIVE Software 10.0 Release Notes](https://developer.nvidia.com/DRIVE/secure/docs/NVIDIA_DRIVE_Software_Release_Notes_10.0.pdf)     
{% endtab %}
{% endtabs %}

