# FAQ

###打包后无法正常使用?
我们建议您使用C++构建工程,构建C++工程的方式是通过 **File -> New C++ Class** 创建一个简单的c++类.

<img src=doc/ueimgs/60ddc60160c52.png width = "50%"  alt="image.png"/>

然后重新 Generate 工程并编译项目,即可正常使用.

###如何快速进行Android打包测试?
- 点击Configure Now

<img src=doc/ueimgs/60dd2f41ae2d9.png width = "50%"  alt="image.png"/>

- 点击Configure Now并,建议取消勾选Include AdMob support for ads.

<img src=doc/ueimgs/60dd37e5d5d51.png width = "50%"  alt="image.png"/>

- (非必要)选择打包支持arm64版本的数据包

<img src=doc/ueimgs/60dd6e82cde70.png width = "50%"  alt="image.png"/>

- 配置好Android需要的SDK与NDK

<img src=doc/ueimgs/sdk.png width = "50%"  alt="image.png"/>



选择File -> Package Project -> Android -> Android(ASTC),选择一个文件夹即可导出.apk


### 如何找到 SpaceTarget/Content

- 在 **ContentBrowser** 右下角的 **View Options** 勾选上 **Show Plugin Content** 

<img src=doc/ueimgs/60dc3fecec49e.png width = "50%"  alt="image.png"/>

- 选择 **SpaceTarget/Content** 文件夹

<img src=doc/ueimgs/60dc403e898ad.png width = "50%"  alt="image.png"/>


### 模型导入出错
- 重新按照 [快速开始](md_document__q_u_i_c_k_s_t_a_r_t.html) 的 **步骤4.** 下载即可.


### 模型颜色丢失

- 自行导入会丢失颜色信息,等待下载完成后,引擎会提示需要导入模型数据,点击 Don't Import

<img src=doc/ueimgs/60ddcb1246dc5.png width = "50%"  alt="image.png"/>

