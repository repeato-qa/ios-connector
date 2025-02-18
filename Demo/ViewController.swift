//
//  ViewController.swift
//  Repeato
//
//  Created by Muhammad Ahad on 21/11/2022.
//

import UIKit

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
    }
    

}

extension ViewController: UITextFieldDelegate {
    
    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        performAction()
        textField.resignFirstResponder()
        return true
    }
    
    func performAction() {
        print("onEnd editing|Done with text filed editing")
    }
}


extension ViewController: UITextViewDelegate {
    
    func textViewDidEndEditing(_ textView: UITextView) {
        print("onEnd editing|Done with text view editing")
    }
}


extension ViewController: UISearchBarDelegate {
    
    func searchBarSearchButtonClicked(_ searchBar: UISearchBar) {
        print("onEnd editing|searchBarSearchButtonClicked")
    }
    func searchBarShouldEndEditing(_ searchBar: UISearchBar) -> Bool {
        print("onEnd editing|searchBarShouldEndEditing")
        return true
    }
    
    func searchBarTextDidEndEditing(_ searchBar: UISearchBar) {
        print("onEnd editing|searchBarTextDidEndEditing")
    }
}
