import QtQuick


//TODO: move this to cpp

QtObject {
    id: _object

    property var data: ({})
    property string endpoint: ''
    property bool authenticate: true

    signal ready(var result)
    signal error(int code, var msg)



    function request(verb, endpoint, body){
        var xhr = new XMLHttpRequest()
        endpoint = endpoint ? endpoint :_object.endpoint

        xhr.onloadend = function() {
            let str = xhr.responseText.toString()
            console.log(str)
            let result = null
            if(str.length > 1)
                try {
                    result = JSON.parse(str)
                }catch(e){
                    _object.error(0, 'No Json resault!!')
                }
            if(xhr.status === 200) {
                _object.ready(result)
            }else {
                _object.error(xhr.status, result && result.message ? result.message : '')
            }
        }
        xhr.open(verb, 'http://localhost:8000/api/' + endpoint)
        xhr.setRequestHeader('Content-Type', 'application/json')
        xhr.setRequestHeader('Accept', 'application/json')
        if(authenticate)
            xhr.setRequestHeader('Authorization', 'Bearer ' + settings.value('bearer'))

        body = body !== undefined ? body : _object.data ? JSON.stringify(_object.data) : ''
        xhr.send(body)
        return xhr
    }

    function get(){
        let endpoint = _object.endpoint
        for (let i in _object.data){
                             endpoint += '/'+_object.data[i]
                             }
        return request('GET', endpoint, '')
    }
    function post(){
        return request('POST')
    }
    function put(){
        return request('PUT')
    }
    function del(){
        let endpoint = _object.endpoint
        for (let i in _object.data){
                             endpoint += '/'+_object.data[i]
                             }
        return request('DELETE', endpoint, '')
    }


}
