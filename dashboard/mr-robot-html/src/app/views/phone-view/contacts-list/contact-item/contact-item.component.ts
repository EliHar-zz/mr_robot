import {Component, OnInit, Input} from '@angular/core';
import {ContactItem} from "./contact-item";

@Component({
  selector: 'mr-contact-item',
  templateUrl: './contact-item.component.html',
  styleUrls: ['./contact-item.component.scss']
})
export class ContactItemComponent implements OnInit {

  @Input() contact : ContactItem;
  constructor() { }

  ngOnInit() {
  }

}
