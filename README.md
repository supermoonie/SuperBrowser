# SuperBrowser
super browser

- [ ] Debug
  - [ ] File
    - [ ] Open
    - [ ] SetContent Ctrl + V
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
      - [ ] 增加 parse
    - [x] Del
    - [x] Clear
    - [x] Del
  - [ ] Cache
    - [ ] SetDirectory
    - [ ] SetMaximumCacheSize
    - [ ] Show CacheSize
    - [ ] Remove
  - [ ] Mail
    - [ ] login
    - [ ] receive

- [ ] 将 Interceptor 移入 Network 中



- [ ] Release
  - [ ] Browser
    - [x] version
    - [x] navigate
    - [x] getWindowBounds
    - [x] setWindowBounds
    - [x] getWindowState
    - [x] setWindowState
    - [x] close
  - [ ] Dom: loadStart List\<QWebElement>
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
    - [ ] setBlockedURLs
    - [x] setProxy
  - [ ] Page
    - [ ] currentUrl
    - [ ] progress
    - [x] getUserAgent
    - [x] setUserAgent
    - [ ] hasAlert
    - [ ] alertText
    - [ ] acceptAlert
    - [ ] hasConfirm
    - [ ] confirmText
    - [ ] acceptConfirm(true/false)
    - [ ] hasPrompt
    - [ ] setPromptInputValue
    - [ ] acceptPrompt(true/false)
    - [ ] setHtml
    - [ ] toHtml
    - [ ] toPlainText
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
    }
}
```



