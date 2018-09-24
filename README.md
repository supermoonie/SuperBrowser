# SuperBrowser
super browser

- 导航
- 代理
- cookie
  - 增加 & 更新
  - 删除 & 删除全部
  - 获取 & 获取全部
- 截图 & pdf
- 截取
- 缓存目录
- 执行JavaScript

{"name":"navigate","parameters":{"url":"https://www.baidu.com"}}boundary---------

{"name":"close"}boundary---------

{"name":"getAllCookies"}boundary---------

{"name":"getAllCookies","parameters":["https://www.baidu.com/"]}boundary---------

{"name":"setCookie","parameters":{"name":"cookieName","value":"balabala","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true}}boundary---------

{"name":"setCookies","parameters":[{"name":"cookieName1","value":"cookieValue1","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true},{"name":"cookieName2","value":"cookieValue2","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true}]}boundary---------