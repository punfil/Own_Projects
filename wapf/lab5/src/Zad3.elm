module Zad3 exposing (main)

import Browser
import Html exposing (Html, div, img, text, h1, h3, a, button, p)
import Html.Attributes exposing (src, alt, href, style)
import Html.Events exposing (onClick)

type alias Photo =
    { id : Int
    , url : String
    , name : String
    , description : String
    , source : String
    , showDetails : Bool
    }

type alias Model =
    { photos : List Photo
    , showDescriptions : Bool
    }

init : Model
init =
    { photos =
        [ { id = 1, url = "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s", name = "Słońce", description = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus lacinia odio vitae vestibulum.", source = "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s", showDetails = True }
        , { id = 2, url = "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s", name = "Ziemia", description = "Suspendisse potenti. Nunc faucibus purus at eros eleifend, vel consequat nunc ultrices." ,source = "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s", showDetails = True }
        , { id = 3, url = "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s", name = "Księżyc", description = "Aenean condimentum justo nec libero pulvinar, ut tincidunt ligula pellentesque.", source = "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s", showDetails = True }
        ]
    , showDescriptions = True
    }

type Msg
    = ToggleDetails Int
    | ToggleDescriptions

update : Msg -> Model -> Model
update msg model =
    case msg of
        ToggleDetails id ->
            { model | photos = List.map (\photo -> if photo.id == id then { photo | showDetails = not photo.showDetails } else photo) model.photos }

        ToggleDescriptions ->
            { model | showDescriptions = not model.showDescriptions }

main : Program () Model Msg
main =
    Browser.sandbox { init = init, update = update, view = view }

view : Model -> Html Msg
view model =
    div []
        ( h1 [] [ text "Galeria" ]
        :: button [ onClick ToggleDescriptions ] [ text (if model.showDescriptions then "Ukryj wszystkie opisy" else "Pokaż wszystkie opisy") ]
        :: List.map (viewSinglePhoto model.showDescriptions) model.photos
        )

viewSinglePhoto : Bool -> Photo -> Html Msg
viewSinglePhoto showDescriptions photo =
    div []
        ( h3 [] [ text photo.name ]
        :: button [ onClick (ToggleDetails photo.id) ] [ text (if photo.showDetails then "Ukryj szczegóły" else "Pokaż szczegóły") ]
        :: (if photo.showDetails then
                [ img [ src photo.url, alt photo.name, style "width" "300px", style "height" "auto" ] []
                , (if showDescriptions then p [] [ text photo.description ] else text "")
                , div []
                    [ text "Source: "
                    , a [ href photo.source ] [ text photo.source ]
                    ]
                ]
            else
                [])
        )
