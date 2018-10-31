# SuperBrowser
super browser

- [ ] Debug
  - [ ] File
    - [ ] Open
  - [x] Navigate
  - [x] Server
    - [x] Start
    - [x] Stop
  - [x] Settings
    - [x] Proxy
    - [x] User-Agent
    - [x] Interceptor
  - [x] Network
    - [x] Extractor
      - [x] show
      - [x] Base64View
        - [x] Base64ToStr
        - [x] Base64ToImg
      - [x] Add
      - [x] Del
      - [x] Clear
    - [x] Cookie
      - [x] show
      - [x] Add
      - [x] Del
      - [x] Clear
      - [x] Del
  - [ ] Cache
    - [ ] SetDirectory
    - [ ] SetMaximumCacheSize
      - [ ] Show CacheSize
    - [ ] Remove

- [ ] 将 Interceptor 移入 Network 中



- [ ] Release
  - [ ] Browser
    - [x] navigate
    - [ ] getWindowBounds
    - [ ] setWindowBounds
    - [ ] close
  - [ ] Dom: loadStart List<QWebElement>
    - [ ] Id
    - [ ] Document
    - [ ] QuerySelector(FindFirst)
    - [ ] QuerySelectorAll(FindAll)
    - [ ] GetGeometry
  - [ ] Network
    - [ ] SetCacheDirectory
    - [ ] ClearCache
    - [ ] DeleteCookie
    - [ ] GetAllCookies
    - [ ] GetCookides
    - [ ] SetCookie
    - [ ] SetCookies
    - [ ] GetResponseBody
    - [ ] GetRequestPostData
    - [ ] SetBlockedURLs
    - [ ] SetUserAgentOverride
    - [ ] SetProxy
  - [ ] Page
    - [ ] CaptureScreenshot
    - [ ] PrintToPDF
    - [ ] Reload
    - [ ] SetDocumentContent
    - [ ] StopLoading
  - [ ] Runtime
    - [ ] Evaluate

```json
{"name":"navigate","parameters":{"url":"https://ip.cn"}}

{"name":"setProxy","parameters":{"host":"127.0.0.1","port":8888,"user":"","password":"","type":"http"}}

{"name":"setUserAgent","parameters":"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/17.17134"}

{"name":"setInterceptor","parameters":"al.jpg"}

{"name":"close"}

{"name":"getAllCookies"}

{"name":"getAllCookies","parameters":["https://www.baidu.com/"]}

{"name":"setCookie","parameters":{"name":"cookieName","value":"balabala","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true}}

{"name":"setCookies","parameters":[{"name":"cookieName1","value":"cookieValue1","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true},{"name":"cookieName2","value":"cookieValue2","domain":"www.baidu.com","path":"/","expires":null,"httpOnly":true,"secure":true}]}

{"name":"deleteCookie","parameters":{"name":"cookieName","url":"https://www.baidu.com/"}}

{"name":"deleteCookies","parameters":[{"name":"cookieName1","url":"https://www.baidu.com/"},{"name":"cookieName2","url":"https://www.baidu.com/"}]}

{"name":"setInterceptors","parameters":["https://s.ip-cdn.com/img/logo.gif"]}

{"name":"getResponse","parameters":".*(day.php).*"}
{"name":"getResponses","parameters":[".*(interested.php).*",".*(day.php).*"]}
```



