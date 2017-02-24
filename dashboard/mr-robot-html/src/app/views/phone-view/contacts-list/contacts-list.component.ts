import { Component, OnInit } from '@angular/core';
import {ContactsService} from "../../service/contacts.service";
import {ContactItem} from "./contact-item/contact-item";

@Component({
  selector: 'mr-contacts-list',
  templateUrl: './contacts-list.component.html',
  styleUrls: ['./contacts-list.component.scss']
})
export class ContactsListComponent implements OnInit {
  public contactsList : ContactItem[];
  constructor(private contactsService : ContactsService) { }

  ngOnInit() {
    this.contactsList = this.contactsService.getContacts;
  }

}
