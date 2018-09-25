# SuperBrowser
super browser

- 导航
- 配置
  - 代理
  - UserAgent
  - 拦截规则
  - 截取规则
- cookie
  - setCookie(cookie)
  - setCookies([cookie])
  - deleteCookie(cookieName, url)
  - deleteCookies([cookieName, url])
  - getCookies()
  - getCookies([url,url])
- 截图 & pdf
- 截取
- 缓存目录
- 执行JavaScript
- 日志输出

```json
{"name":"navigate","parameters":{"url":"https://ip.cn/"}}boundary---------

{"name":"setProxy","parameters":{"host":"127.0.0.1","port":8888,"user":"","password":"","type":"http"}}boundary---------

{"name":"close"}boundary---------

{"name":"getAllCookies"}boundary---------

{"name":"getAllCookies","parameters":["https://www.baidu.com/"]}boundary---------

{"name":"setCookie","parameters":{"name":"cookieName","value":"balabala","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true}}boundary---------

{"name":"setCookies","parameters":[{"name":"cookieName1","value":"cookieValue1","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true},{"name":"cookieName2","value":"cookieValue2","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true}]}boundary---------

{"name":"deleteCookie","parameters":{"name":"cookieName","url":"https://www.baidu.com/"}}boundary---------

{"name":"deleteCookies","parameters":[{"name":"cookieName1","url":"https://www.baidu.com/"},{"name":"cookieName2","url":"https://www.baidu.com/"}]}boundary---------
```



