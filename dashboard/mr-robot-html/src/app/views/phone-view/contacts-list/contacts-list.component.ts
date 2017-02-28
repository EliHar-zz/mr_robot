import { Component, OnInit } from '@angular/core';
import {ContactsService} from "../../service/contacts.service";
import {ContactItem} from "./contact-item/contact-item";
import {DialService} from "../../service/dial.service";

@Component({
  selector: 'mr-contacts-list',
  templateUrl: './contacts-list.component.html',
  styleUrls: ['./contacts-list.component.scss']
})
export class ContactsListComponent implements OnInit {
  public contactsList : ContactItem[];
  constructor(private contactsService : ContactsService, private dialService : DialService) { }

  ngOnInit() {
    this.contactsList = this.contactsService.getContacts;
  }

  public onContactSelect(contact : ContactItem) {
    if(contact.getNumbers.length == 1) {
      this.dialService.setDialNumber(contact.getNumbers[0]);
      this.dialService.disableShouldSelectNumber();
    } else if(contact.getNumbers.length > 1) {
      this.dialService.enableShouldSelectNumber(contact);
    }
  }
}
