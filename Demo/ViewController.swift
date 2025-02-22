//
//  ViewController.swift
//  Repeato
//
//  Created by Muhammad Ahad on 21/11/2022.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var theTexfield: UITextField!
    
    @IBOutlet weak var theTextView: UITextView!
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        
        let toolbar = UIToolbar()
                let flexSpace = UIBarButtonItem(barButtonSystemItem: .flexibleSpace,
                                                target: nil, action: nil)
                let doneButton = UIBarButtonItem(title: "HideKeyBoard", style: .done,
                                                 target: self, action: #selector(doneButtonTapped))
                
                toolbar.setItems([flexSpace, doneButton], animated: true)
                toolbar.sizeToFit()
                
        theTextView.inputAccessoryView = toolbar
        theTexfield.inputAccessoryView = toolbar
    }
    
    @objc func doneButtonTapped() {
            view.endEditing(true)
        }

}

