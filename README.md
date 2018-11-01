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
    - [ ] Develop Tools
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
    - [x] getWindowBounds
    - [x] setWindowBounds
    - [x] getWindowState
    - [x] setWindowState
    - [x] close
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
    - [x] SetProxy
  - [ ] Page
    - [x] getUserAgent
    - [x] setUserAgent
    - [ ] CaptureScreenshot
    - [ ] PrintToPDF
    - [ ] Reload
    - [ ] SetDocumentContent
    - [ ] StopLoading
  - [ ] Runtime
    - [ ] Evaluate

```json
{
    "version": {
        "major": 1,
        "minor": 0
    },
    "domains": {
        "domain": "Browser",
        "types": [
            {
                "id": "WindowState",
                "type": "string",
                "enum": [
                    "normal",
                    "minimized",
                    "maximized",
                    "fullscreen",
                    "active"
                ]
            }
        ],
        "commands": [
            {
                "name": "navigate",
                "parameters": {
                    "url": "http://httpbin.org/get"
                }
            },
            {
                "name": "getWindowBounds"
            }
        ]
    }
}
```



