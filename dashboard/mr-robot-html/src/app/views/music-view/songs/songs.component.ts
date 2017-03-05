import { Component, OnInit } from '@angular/core';
import {MusicService} from "../../service/music.service";
import {MusicItem} from "../music-item/music-item";

@Component({
  selector: 'mr-songs',
  templateUrl: './songs.component.html',
  styleUrls: ['./songs.component.scss']
})
export class SongsComponent implements OnInit {

  public allSongs : MusicItem[];
  constructor(private musicService : MusicService) { }

  ngOnInit() {
    this.allSongs = this.musicService.getSongs;
  }
}
