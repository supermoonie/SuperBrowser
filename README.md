# SuperBrowser
super browser

- 导航
- 配置
  - setProxy(proxy)
  - setUserAgent(ua)
  - setInterceptor(interceptor)
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
{"name":"navigate","parameters":{"url":"http://pianke.me/pages/timeline/timeline.html"}}boundary---------

{"name":"setProxy","parameters":{"host":"127.0.0.1","port":8888,"user":"","password":"","type":"http"}}boundary---------

{"name":"setUserAgent","parameters":"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/17.17134"}boundary---------

{"name":"setInterceptor","parameters":"al.jpg"}boundary---------

{"name":"close"}boundary---------

{"name":"getAllCookies"}boundary---------

{"name":"getAllCookies","parameters":["https://www.baidu.com/"]}boundary---------

{"name":"setCookie","parameters":{"name":"cookieName","value":"balabala","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true}}boundary---------

{"name":"setCookies","parameters":[{"name":"cookieName1","value":"cookieValue1","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true},{"name":"cookieName2","value":"cookieValue2","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true}]}boundary---------

{"name":"deleteCookie","parameters":{"name":"cookieName","url":"https://www.baidu.com/"}}boundary---------

{"name":"deleteCookies","parameters":[{"name":"cookieName1","url":"https://www.baidu.com/"},{"name":"cookieName2","url":"https://www.baidu.com/"}]}boundary---------

{"name":"addExtractor","parameters":".*(listByTag.php).*"}boundary---------
```



