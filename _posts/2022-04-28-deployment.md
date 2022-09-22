---
layout: post
title:  "应用发布策略"
date:   2022-04-28
categories: ops
---

| 名称           | 英文名称    | 说明                                                                                 | 优点                                       | 缺点                                             |
|----------------|-------------|--------------------------------------------------------------------------------------|--------------------------------------------|--------------------------------------------------|
| 蓝绿           | BlueGreen   | 存在两套独立的环境：蓝（旧）绿（新），依靠前面的LB决定切到哪个环境，发现不行再切回去 | 减少中断时间、支持快速回滚                 | 2倍硬件                                          |
| 滚动           | Rolling     | 慢慢逐个替换老应用实例，新实例一旦health check通过之后，老的就被删掉                 | 解决蓝绿的2倍硬件的问题，用一套资源即可    | 滚动发布期间，整个系统不稳定，需要有流量控制手段 |
| 金丝雀（灰度） | Canary      | 和蓝绿类似，先搭建一套新版本环境，在LB上先放小部分用户进来，逐步牵引                 | 用真实环境在测试、只影响部分用户、快速回滚 | 2倍硬件                                          |
| A/B测试        | A/B testing | 用来验证某个想法是否符合预期，无新旧之分，多个版本同时运行，看哪个接受程度更高       | 根据用户的喜欢选择最好的方案               | 同时维护两套                                     |

Reference:

- https://martinfowler.com/bliki/BlueGreenDeployment.html
- https://martinfowler.com/bliki/CanaryRelease.html
- https://blog.christianposta.com/deploy/blue-green-deployments-a-b-testing-and-canary-releases/
