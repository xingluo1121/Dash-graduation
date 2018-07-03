# 360全景视频自适应流媒体技术研究

## Research on Panoramic Video Streaming over DASH

### 摘要

### 关键字： 流媒体，自适应算法， 360全景视频， 视频编码

### Abstract

### key words: streaming, Adapation Algorithm, 360 Video, Video Coding

## 第一章 绪论

### 1.1 研究背景


流媒体业务的迅猛发展
https://www.cisco.com/c/zh_cn/about/press/corporate-news/2017/02-08.html
https://www.cisco.com/c/en/us/solutions/collateral/service-provider/visual-networking-index-vni/mobile-white-paper-c11-520862.html?CAMPAIGN=Mobile+VNI+2017&COUNTRY_SITE=us&POSITION=Press+Release&REFERRING_SITE=PR&CREATIVE=PR+to+MVNI+white+paper
http://www.ccidcom.com/yaowen/20170606/B5SLJft2gS46myZQT14xr1dknb3o0.html
Netfliex Youtube hulu twitch / 腾讯视频 优酷土豆 爱奇艺
1、视频业务在线化、移动化、高清化（大视频时代）
http://www.miit.gov.cn/newweb/n1146312/n1146909/n1146991/n1648534/c3489195/content.html
http://tc.people.com.cn/n1/2016/0627/c183008-28480272.html
https://www.huawei.com/minisite/ubbf/2016/cn/newsinfo1-16.html
2、360全景视频业务异军突起（VR元年）
Facebook live360， YouTube 360 video，国内视频网站/社交网站陆续推出360视频点播直播业务
虚拟现实VR业务爆发：内容支撑360全景视频
流媒体服务对产业界
http://carrier.huawei.com/cn/trends-and-insights/win-video
设备//内容//网络
=>高带宽 大流量 = 时变的无线网络
=>新型业务形态 = 内容制作分发，兼容现有设备，匹配现有网络能力
本文的思路：流媒体业务优化改进->360全景视频实现和改进
https://helpx.adobe.com/cn/premiere-pro/kb/work-with-vr.html
“头号玩家”

### 1.2 研究现状和意义

#### 1.2.1 自适应流媒体技术研究现状

流媒体技术
https://www.vocal.com/video/video-streaming-technology/
https://www.quora.com/What-are-the-best-video-streaming-technology
https://www.quora.com/Who-has-the-best-live-streaming-video-technology-in-terms-of-quality-scalability-and-UI

https://www.quora.com/What-are-the-leading-live-video-streaming-companies
MPEG-DASH
HLS
RTMP
HDS
MMS
RTP/RTSP
HTTP-flv

MPEG-DASH => Adaption algo / QoE
Algo-> bw,buf,hy
QoE->


#### 1.2.2 360全景视频传输技术研究现状

360video tech stack
拍摄 投影变换 编码 传输 解码 投影变换 播放
投影变换 ERP CMP 。。。
编码 domain compress；mpeg4封装方式变化
传输 传统、非对称投影+编码 分块编码 = 单流 多流 DASH
运动预测 带宽分配 QoE评价

#### 1.2.3 研究意义

自适应流媒体改进不断提升QoE
大视频时代的机遇与挑战
360全景视频流媒体提供全新的感官体验
VR全景视频的愿景

### 1.3 本文研究工作

#### 1.3.1 基于分片感知和动态缓存的自适应流媒体

目标： 无卡顿；高质量；避免质量切换；快启动（缓存上线随着时间线性增加）

分片感知：实时码率感知
动态缓存：带宽估计+缓存变化反馈 hy
与BLOA相同，并不强调缓存不变，恰恰相反，利用其动态性，减少瞬时码率变换和带宽波动对播放质量的影响
引入概念：VBR编码-固定质量编码-QoE的影响=>变比特率编码提升流媒体服务的质量
          因此将 自适应比特率问题 转化为 自适应quality
          引入概念：crf，PSNR
          Q_delta（瞬时PSNR-平均PNSR）B_delta（瞬时Bitrate-平均Bitrate）
          1.对于高出平均码率的segment，优先级：a:Q_delta/B_delta：每额外增加单位比特率的开销增加的PSNR（递减）f(x) （可能与x轴正半轴有交点）
                                             b:B_delta/BW->t_extra->cal-> 卡顿风险增加（递增）g(x)
                                             Normal_a,Normal_b: g(f(x)) 拐点：将卡顿风险控制在一定范围内提升Quality
          由此导致缓存的波动->可控的波动（为了提升质量）
          2.对于低于平均码率的segment，同样有优先级：为高质量的segment调高缓存上限
          码率切换的annoying也改用psnr来量化，码率越高，码率切换带来的提升/降低越小，对切换的容忍度越高
对于平均码率为 B_avg(Mbps) 平均质量 Q_avg

#### 1.3.2 基于CMP投影的360视频传输

CMP投影质量保持的好（畸变）应用广泛度仅次于ERP，分块合理（参考文献，相比于正八面体正二十面体等），实现简单（六块的投影运算相同，相比与TSP等）
因此就变成了六块的DASH
六块之间的带宽分配问题（在上述的基础上）
优先级plus+运动预测模型（假设那种傻逼分布）
确定任意时刻的视点位置：1.每块进入视野的面积 2. 每块距离视点的位置（权重） 3.辅助块与主块质量差会引入多少的decreasement  -> 优先级plus

#### 1.3.3 自适应流媒体仿真平台搭建

傻逼NS3 自适应流媒体仿真平台+分块传输360全景视频搭建 系统状态机

### 1.5 论文结构

## 第二章 基于HTTP的动态自适应流媒体技术

### 2.1 视频编码技术概述

IBP帧和GOP概念:GOP越大，压缩比越高；流媒体中GOP需要时封闭的，切片需要GOP的整数倍
VQM PSNR:视频质量评价,PSNR基本上可以表征用户体验，相比于比特率表示更合理（PSNR和bitrate不是线性关系，bitrate和视频内容高度相关）
流媒体实现中对于视频编码的要求
CBR编码和CBR编码：结合VQM说明CBR才是正确的，用crf参数保证视频流具有稳定的quality

### 2.2 自适应流媒体技术发展与应用

HTTTP-based,MPEG-DASH HLS
RTSP-based,RTP/RTSP

差异不大各有千秋，我大MPEG一统天下（然而并不是）

### 2.2 DASH技术标准概述

垃圾MPEG-DASH出来骗钱啦，架构图拿出来骗人

#### 2.2.1 媒体描述文件（MPD）

XML文件分析

#### 2.2.2 媒体切片文件（SF）

segment format要求分析
GOP/IDR帧的问题

#### 2.2.3 自适应算法（ABA）

Can accurate bandwidth estimate improving QoE
 of DASH?
Bandwidth-baesd:festive
Buffer-based:Bloa
Hybrid-Algo:tobasco
Newly-dev:各种奇葩理论
在座的各位都是垃圾（跑）

### 2.3 360视频编码与传输

异形picture不兼容传统videocoding的问题
所以相比于传统流媒体
额外的编码工作，额外的传输方案

#### 2.3.1 投影变换和编码

额外的编码工作：
videocoding的难点巴拉巴拉各种傻逼玩意
projection投影变化（ERP，CMP，TSP。。。。）
(JEVT工作组对不起，你们的东西我都要抄过来
MPEG2和MPEG4封装问题)
分块编码不能无限划分那些分成几十块的傻逼你们都去死吧

#### 2.3.2 360视频质量评价

360视频质量评价:WSPSNR SPSNR 等等
PSNR。真香。

#### 2.3.3 全景视频传输技术

单流：传统单流；FOV流
多流：Tile流（研究热点）
带来的问题：带宽分配和衡量标准设定=>>>>优化目标？

## 第三章 基于ns3的自适应流媒体仿真平台搭建

ns3真香警告

## 第四章 基于分片感知和动态缓存的自适应算法

### 4.1 动态比特率编码与分片感知算法

### 4.2 动态缓存控制策略

### 4.3 算法仿真和性能评估

## 第五章 基于分块流的360视频自适应算法

### 5.1 基于概率模型的码率分配

### 5.2 360视频自适应算法

### 5.3 算法仿真和性能评估

## 第六章 总结和展望

### 6.1 本文工作总结

### 6.2 研究方向展望

#### 参考文献