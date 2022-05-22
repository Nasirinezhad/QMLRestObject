# QMLRestObject

I'll compelate this!


    qmlRegisterType<REST>("Rest", 1,1,"Rest");



    Rest {
        id: newProduct
        data: ({
            name: name_inp.text
            slug: slug_inp.text
            price: price_inp.text
            .
            .
            .
        })
        endpoint: 'product'
        onReady: (res)=> {
                    //successful
                 }
        onError: (code, msg) => {
                     switch (code) {
                         case 403:
    
                         break;
                         case 0:
                         
                         break;
                         default:
                         
                         break;
                     }
                 }
    }