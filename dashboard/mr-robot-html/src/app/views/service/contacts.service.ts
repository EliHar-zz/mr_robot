import { Injectable } from '@angular/core';
import {ContactItem} from "../phone-view/contacts-list/contact-item/contact-item";

@Injectable()
export class ContactsService {
  public contacts : ContactItem[];
  constructor() {
    this.contacts = [];
  }

  public addContact(contactItem : ContactItem) : void {
    this.contacts.push(contactItem)
  }

  public get getContacts() : ContactItem[] {
    return this.contacts;
  }
}
