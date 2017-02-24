import {Component, OnInit} from '@angular/core';
import {ViewDataService} from "./views/service/view-data.service";
import {AppItemClass} from "./views/apps-view/app-item/app-item";
import {ContactsService} from "./views/service/contacts.service";
import {ContactItem} from "./views/phone-view/contacts-list/contact-item/contact-item";

@Component({
  selector: 'mr-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {

  constructor(public viewDataService : ViewDataService, public contactsService : ContactsService){}

  private _addDefaultViews() {
    this.viewDataService.addView(new AppItemClass("Map", "fa fa-map", ['/open','map']));
    this.viewDataService.addView(new AppItemClass("Road", "fa fa-road", ['/open', 'road']));
    this.viewDataService.addView(new AppItemClass("Phone", "fa fa-phone", ['/open', 'phone']));
    this.viewDataService.addView(new AppItemClass("Music", "fa fa-music", ['/open', 'music']));
    this.viewDataService.addView(new AppItemClass("Control", "fa fa-gamepad", ['/open', 'control']));
  }

  private _addDefaultContacts() {
    this.contactsService.addContact(new ContactItem("Inez Simmons", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Vera Hill", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Mae Garcia", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Olivia Lindsey", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Richard Bailey", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Joe Mitchell", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Owen Jefferson", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Sheila Pierce", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Susan Hill", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Kara Richards", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Milton Gibson", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Neil Reyes", [123456789,987654321]));
    this.contactsService.addContact(new ContactItem("Jean Garza", [123456789,987654321]));

  }

  ngOnInit() {
    this._addDefaultViews();
    this._addDefaultContacts();
  }
}
